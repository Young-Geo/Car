
(cl:in-package :asdf)

(defsystem "node_ultrasonic-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "ultr" :depends-on ("_package_ultr"))
    (:file "_package_ultr" :depends-on ("_package"))
  ))