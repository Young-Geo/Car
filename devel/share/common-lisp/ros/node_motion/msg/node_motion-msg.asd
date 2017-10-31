
(cl:in-package :asdf)

(defsystem "node_motion-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "motion" :depends-on ("_package_motion"))
    (:file "_package_motion" :depends-on ("_package"))
  ))