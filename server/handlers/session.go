package handlers

import (
	"fmt"
	"log"
	"net/http"
	"strconv"

	"github.com/google/uuid"
	"github.com/julienschmidt/httprouter"
	"github.com/suyashkumar/bike/server/store"
)

const (
	sessionIDParam = "session_id"
	revsParam      = "revs"
	startTimeParam = "start_time"
	endTimeParam   = "end_time"
)

// Context holds onto various pieces of global context that are injected into
// the handlers.
type Context struct {
	StorageClient *store.Client
}

type Handle func(w http.ResponseWriter, r *http.Request, ps httprouter.Params, ctx Context)

func NewSession(w http.ResponseWriter, r *http.Request, ps httprouter.Params, ctx Context) {
	id := uuid.New()
	if _, err := fmt.Fprintf(w, "%s", id.String()); err != nil {
		w.WriteHeader(500)
	}
	log.Println("new session: ", id)
}

func AddRevolutions(w http.ResponseWriter, r *http.Request, ps httprouter.Params, ctx Context) {
	params := r.URL.Query()

	revs, err := strconv.Atoi(params.Get(revsParam))
	if err != nil {
		w.WriteHeader(400)
		return
	}

	startTime, err := strconv.Atoi(params.Get(startTimeParam))
	if err != nil {
		w.WriteHeader(400)
		return
	}

	endTime, err := strconv.Atoi(params.Get(endTimeParam))
	if err != nil {
		w.WriteHeader(400)
		return
	}

	data := store.RevolutionsBatch{
		Revolutions: revs,
		SessionID:   params.Get(sessionIDParam),
		StartMillis: startTime,
		EndMillis:   endTime,
	}
	log.Printf("Info: %+v\n", data)

	if err := ctx.StorageClient.WriteRevolutions(data); err != nil {
		log.Println(err)
		w.WriteHeader(400)
		return
	}
}

func TotalDistance(w http.ResponseWriter, r *http.Request, ps httprouter.Params, ctx Context) {
	sessionID := r.URL.Query().Get(sessionIDParam)
	data, err := ctx.StorageClient.ReadRevolutions(sessionID)
	if err != nil {
		w.WriteHeader(500)
		return
	}

	total := 0
	for _, batch := range data {
		log.Printf("count: %+v", batch)
		total += batch.Revolutions
	}

	header := w.Header()
	header.Set("Access-Control-Allow-Origin", "*")

	if _, err := fmt.Fprintf(w, "%d", total); err != nil {
		w.WriteHeader(500)
		return
	}
	w.WriteHeader(200)

}
