// Package store is an extremely simple storage abstraction for revolution data
// collected in biking sessions. It doesn't use a database, just writes to
// files on the host filesystem.
package store

import (
	"bytes"
	"encoding/gob"
	"io"
	"os"
	"path"
	"sync"
)

type Client struct {
	dir string
	mu  sync.Mutex
}

func NewClient(dir string) *Client {
	return &Client{
		dir: dir,
	}
}

type RevolutionsBatch struct {
	SessionID   string
	Revolutions int
	StartMillis int
	EndMillis   int
}

func (c *Client) WriteRevolutions(batch RevolutionsBatch) error {
	c.mu.Lock()
	defer c.mu.Unlock()

	skipHeader := true
	filepath := path.Join(c.dir, batch.SessionID)
	if _, err := os.Stat(filepath); os.IsNotExist(err) {
		// This is the first
		skipHeader = false
	}

	f, err := os.OpenFile(path.Join(c.dir, batch.SessionID), os.O_APPEND|os.O_CREATE|os.O_WRONLY, 0644)
	if err != nil {
		return err
	}
	defer f.Close()
	return encode(f, batch, skipHeader)
}

func (c *Client) ReadRevolutions(sessionID string) ([]RevolutionsBatch, error) {
	c.mu.Lock()
	defer c.mu.Unlock()
	f, err := os.Open(path.Join(c.dir, sessionID))
	if err != nil {
		return nil, err
	}
	defer f.Close()

	dec := gob.NewDecoder(f)
	data := make([]RevolutionsBatch, 0, 100) // guess 100 as reasonable init cap

	for {
		var buf RevolutionsBatch
		if err := dec.Decode(&buf); err != nil {
			break
		}
		data = append(data, buf)
	}

	if err != io.EOF {
		return data, err
	}

	return data, nil
}

// wrappedWriter is a struct that wraps a writer, which can be switched out
// later to allow skipping the gob header write to file.
type wrappedWriter struct {
	w io.Writer
}

func (w *wrappedWriter) Write(p []byte) (int, error) {
	return w.w.Write(p)
}

func encode(w io.Writer, r RevolutionsBatch, skipHeader bool) error {
	if skipHeader {
		return encodeWithSkippedHeader(w, r)
	}

	enc := gob.NewEncoder(w)
	if err := enc.Encode(r); err != nil {
		return err
	}
	return nil
}

func encodeWithSkippedHeader(w io.Writer, r RevolutionsBatch) error {
	ww := &wrappedWriter{&bytes.Buffer{}}
	enc := gob.NewEncoder(ww)
	// To skip writing the header, we write out a dummy entry, and then swap out
	// the underlying writer to the actual writer we wish to write to.
	if err := enc.Encode(RevolutionsBatch{}); err != nil {
		return err
	}
	ww.w = w
	if err := enc.Encode(r); err != nil {
		return err
	}
	return nil
}
