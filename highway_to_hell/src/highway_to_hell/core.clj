(ns highway-to-hell.core
  (:use [highway-to-hell.wiki])
  (:gen-class))

(defn parse-data [data]
  (clojure.string/split data #"\s"))


(defn -main
  "I don't do a whole lot ... yet."
  [page-name & args]
  (println (clojure.string/join ", " (parse-data (clojure.string/lower-case (remove-html-tags (get-text :uk page-name)))))))
