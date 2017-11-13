
(cl:in-package :asdf)

(defsystem "node_camera-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "camera" :depends-on ("_package_camera"))
    (:file "_package_camera" :depends-on ("_package"))
  ))