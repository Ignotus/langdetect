(defproject highway_to_hell "0.1.0-SNAPSHOT"
  :description "Blah blah blah"
  :url "https://root.org.ua"
  :license {:name "MIT"
            :url "http://opensource.org/licenses/MIT"}
  :dependencies [[org.clojure/clojure "1.6.0"]]
  :main ^:skip-aot highway-to-hell.core
  :target-path "target/%s"
  :profiles {:uberjar {:aot :all}})
