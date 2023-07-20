
(cl:in-package :asdf)

(defsystem "image-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "ping" :depends-on ("_package_ping"))
    (:file "_package_ping" :depends-on ("_package"))
  ))