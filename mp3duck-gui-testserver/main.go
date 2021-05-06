package main

import (
	"fmt"
	"io"
	"io/ioutil"
	"net/http"
	"os"
	"strings"

	"github.com/rs/zerolog"
	"github.com/rs/zerolog/log"
)

func main() {

	zerolog.TimeFieldFormat = zerolog.TimeFormatUnixMs
	log.Logger = log.Output(zerolog.ConsoleWriter{Out: os.Stderr})

	http.HandleFunc("/api1/file/", func(w http.ResponseWriter, r *http.Request) {
		GetFileHandler("api1", w, r)
	})

	http.HandleFunc("/api2/file/", func(w http.ResponseWriter, r *http.Request) {
		GetFileHandler("api2", w, r)
	})

	http.ListenAndServe(":8088", nil)
}

func GetFileHandler(tag string, w http.ResponseWriter, r *http.Request) {

	local := "./" + tag + "/"
	api := "/" + tag + "/file/"

	log.Info().
		Str("method", r.Method).
		Str("uri", r.RequestURI).
		Msg("HTTP")

	w.Header().Add("Access-Control-Allow-Origin", "*")
	w.Header().Add("Access-Control-Allow-Methods", "*")
	w.Header().Add("Access-Control-Allow-Headers", "*")
	w.Header().Add("Connection", "close")

	if r.Method == "OPTIONS" {

		w.WriteHeader(200)

	} else if r.Method == "GET" {

		filename := strings.TrimPrefix(r.URL.Path, api)

		file, err := os.Open(local + filename)
		if err != nil {
			http.Error(w, "File could not be found on SD-card", 404)
			return
		}
		defer file.Close()
		stat, err := file.Stat()
		if err != nil {
			http.Error(w, "Error statting file", 500)
			return
		}

		w.Header().Add("Content-Type", "application/octet-stream")
		w.Header().Add("Content-Length", fmt.Sprintf("%d", stat.Size()))

		_, err = io.Copy(w, file)
		if err != nil {
			http.Error(w, "", 500)
			log.Error().Msg("Error sending file")
			return
		}

	} else if r.Method == "POST" {

		res, err := ioutil.ReadAll(r.Body)
		if err != nil {
			http.Error(w, "", 500)
			log.Error().Err(err).Msg("errer")
			return
		}

		log.Info().Bytes("bytes", res).Msg("data")

		/*
			err := r.ParseMultipartForm(50000000)
			if err != nil {
				http.Error(w, "", 500)
				log.Error().Err(err).Msg("Error parsing multipart form")
				return
			}

			file, header, err := r.FormFile("file")
			if err != nil {
				http.Error(w, "", 500)
				log.Error().Err(err).Msg("Error getting multipart file")
				return
			}
			defer file.Close()

			buf := bytes.NewBuffer(nil)
			_, err = io.Copy(buf, file)
			if err != nil {
				http.Error(w, "", 500)
				log.Error().Msg("Error copying file")
				return
			}

			outfile, err := os.OpenFile(local+header.Filename, os.O_CREATE+os.O_WRONLY, 0664)
			if err != nil {
				http.Error(w, "", 500)
				log.Error().Msg("Error creating file")
				return
			}
			defer outfile.Close()

			outfile.Write(buf.Bytes())
		*/
		w.WriteHeader(201)

	} else {

		http.Error(w, "Method not supported", 405)
	}

	return

}
