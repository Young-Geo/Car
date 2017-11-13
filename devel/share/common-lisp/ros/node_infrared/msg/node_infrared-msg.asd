
(cl:in-package :asdf)

(defsystem "node_infrared-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "infr" :depends-on ("_package_infr"))
    (:file "_package_infr" :depends-on ("_package"))
  ))