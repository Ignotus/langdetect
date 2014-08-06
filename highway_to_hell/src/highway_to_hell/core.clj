(ns highway-to-hell.core
  (:use [highway-to-hell.wiki])
  (:gen-class))

(defn -main
  "I don't do a whole lot ... yet."
  [page-name & args]
  (println (remove-html-tags (get-text :uk page-name))))
