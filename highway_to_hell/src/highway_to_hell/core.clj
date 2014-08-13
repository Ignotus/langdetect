(ns highway-to-hell.core
  (:use [highway-to-hell.wiki])
  (:gen-class))

(defn parse-data [data]
  (do
    (clojure.string/split data #"(\s+([,.»«:%'\[\])(-+—/\\]*\s*)*)|(([,.»«:%'\[\])(-+—/\\]+\s*)+)")))

(defn -main
  [page-name & args]
  (println (clojure.string/join "\n" (parse-data (clojure.string/lower-case (slurp page-name))))))
