;;(load "~/quicklisp/setup.lisp")
;;(ql:quickload :cffi)
;;
;;(asdf:load-system :cffi)

(defpackage :posix-shm
  (:use :common-lisp :cffi)
  (:export
   :shm-open
   :shm-unlink
   :ftruncate
   :truncate
   :mmap
   :munmap
   :tryit
   :test
   ))
   
(in-package :posix-shm)

(defconstant O_CREAT 512)
(defconstant O_RDONLY 0)
(defconstant O_RDWR 2)
(defconstant S_IRUSR 256)
(defconstant S_IWUSR 128)
(defconstant PROT_READ 1)
(defconstant PROT_WRITE 2)
(defconstant MAP_SHARED 1)

;;int shm_open(const char *name, int oflag, ...);
(defcfun "shm_open" :int (name :string) (flags :int) (mode :int))
;;int shm_unlink(const char *name);
(defcfun "shm_unlink" :int (name :string))
;;int munmap(void *addr, size_t len);
(defcfun "munmap" :int (addr :pointer) (len :int))
;;void * mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset);
(defcfun "mmap" :pointer (addr :pointer) (len :int) (prot :int) (flags :int) (fd :int) (offset :int))
;;int ftruncate(int fildes, off_t length);
;;(defcfun "ftruncate" :int (fildes :int) (length :int))
;;(defcfun "truncate" :int (name :string) (length :int))

(defcfun "strerror" :string (err :int))
(defcvar "errno" :int)

(defun test ()
  "Test posix-shm functions"
  (format t "~&;; CFFI Test/Lisp version ~A ~A:~%"
	  (lisp-implementation-type)
	  (lisp-implementation-version))
  (shm-open "HELLO" :O_RDWR :S_IWUSR)
  (format t "~&;; shm-open:         ~A~%" (strerror *errno*))
  (shm-unlink "HELLO")	 
  (format t "~&;; shm-close:        ~A~%" (strerror *errno*))
  (force-output))

(defun tryit ()
  (let* ((fd (shm-open "pixels" O_RDWR (logior S_IWUSR S_IRUSR)))
	 (mem (foreign-alloc :unsigned-int :count (* 480 640)))
	 (ptr (mmap mem (* 480 640 4) PROT_WRITE MAP_SHARED fd 0)))

    (loop for i below (* 480 640)
	 do (setf (mem-aref ptr :unsigned-int i) #xFF000000))
    
    (munmap ptr (* 480 640 4))
    (foreign-free mem)
    ;(shm-unlink "pixels")
    ))
