
(cl:in-package :asdf)

(defsystem "node_client-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "client" :depends-on ("_package_client"))
    (:file "_package_client" :depends-on ("_package"))
  ))