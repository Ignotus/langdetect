(ns highway-to-hell.wiki
  (:use [net.cgrand.enlive-html])
  (:require [jsoup.soup]))

(defn- get-url [country article-name]
  (apply str
    ["http://"
     (name country)
     ".wikipedia.org/w/api.php?action=query&prop=extracts&format=xml&utf8&titles="
     article-name]))

(defn get-text [country article-name]
  (let [url
        (get-url country article-name)
        [{[_ {[{[{[{[data] :content}] :content}] :content}] :content}] :content}]
        (html-resource (java.net.URL. url))]
      data 
      ))

(defn remove-html-tags [content]
  (.text (jsoup.soup/parse content)))
