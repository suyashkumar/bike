package main

import (
	"flag"
	"log"
	"net/http"

	"github.com/julienschmidt/httprouter"
	"github.com/suyashkumar/bike/server/handlers"
	"github.com/suyashkumar/bike/server/store"
)

var domain = flag.String("domain", "",
	"domain used for this currently running instance (enables SSL, and mints certs through LetsEncrypt")

func main() {
	flag.Parse()
	log.Fatal(http.ListenAndServe(":8000", buildRouter()))
}

func buildRouter() *httprouter.Router {
	router := httprouter.New()

	ctx := handlers.Context{StorageClient: store.NewClient("data")}

	router.GET("/api/new_session", wrapWithContext(handlers.NewSession, ctx))
	router.GET("/api/add", wrapWithContext(handlers.AddRevolutions, ctx))
	router.GET("/api/distance", wrapWithContext(handlers.TotalDistance, ctx))

	configureCORS(router)
	return router
}

func wrapWithContext(handle handlers.Handle, ctx handlers.Context) httprouter.Handle {
	return func(w http.ResponseWriter, r *http.Request, ps httprouter.Params) {
		handle(w, r, ps, ctx)
	}
}

func configureCORS(router *httprouter.Router) {
	router.GlobalOPTIONS = http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		// Set CORS headers
		header := w.Header()
		header.Set("Access-Control-Allow-Methods", header.Get("Allow"))
		header.Set("Access-Control-Allow-Origin", "http://localhost:3000")
	})
}
