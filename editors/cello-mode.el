;;; cello-mode.el --- Major mode for editting C source code that uses libcello
;; 
;; Filename: cello-mode.el
;; Description: Major mode for editting C source code that uses libcello
;; Author: Jordon Biondo <biondoj@mail.gvsu.edu>
;; Created: Sat Sep 14 21:39:32 2013 (-0400)
;; Version: 0.1.1
;; Package-Requires: ()
;; Last-Updated: Sat Sep 14 21:41:54 2013 (-0400)
;;           By: Jordon Biondo
;;     Update #: 3
;; URL: http://github.com/jordonbiondo/cello-mode.git
;; Keywords: c, languages, tools, cello
;; Compatibility: 
;; 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; 
;;; Commentary: 
;; Major mode for editting C source code that uses libcello
;; 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; 
;;; Change Log:
;;
;;  Sat Sep 14 21:40:59 EDT 2013: creation
;; 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; 
;; This program is free software; you can redistribute it and/or
;; modify it under the terms of the GNU General Public License as
;; published by the Free Software Foundation; either version 3, or
;; (at your option) any later version.
;; 
;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
;; General Public License for more details.
;; 
;; You should have received a copy of the GNU General Public License
;; along with this program; see the file COPYING.  If not, write to
;; the Free Software Foundation, Inc., 51 Franklin Street, Fifth
;; Floor, Boston, MA 02110-1301, USA.
;; 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;; Code:

(defconst cello-home-url "libcello.org"
  "libCello's homepage.")

(defun cello-browse-doc-at-point()
  ;; TODO: hackish, only works on types
  "Browse the libCello docs for the thing at point."
  (interactive)
  (browse-url (format "http://%s/reference/%s" 
		      cello-home-url 
		      (downcase (symbol-name (symbol-at-point))))))
  

(defconst cello-font-lock-keywords
  `(("\\(\\$\\)\\( *( *\\)\\(\[A-Z][A-Za-z_0-9]*\\)"
      (1 font-lock-constant-face)
      (3 font-lock-type-face))
    ;; keywords
    (,(regexp-opt (list "var" "is" "not" "and" "or" "foreach" "try" "catch" "throw" "if_eq" 
			"if_neq" "if_gt" "if_lt" "if_ge" "if_le" "as" "size_t" "true" "false" 
			"va_list*" "var*" "va_list"  "volatile" "module" "class" "data" 
			"instance" "methods" "methods_begin" "method" "methods_end" "defined" 
			"lambda" "lambda_id" "lambda_const" "lambda_compose" "lambda_flip" 
			"lambda_partial" "lambda_partial_l" "lambda_partial_r" "lambda_void" 
			"lambda_uncurry" "lambda_void_uncurry" "lambda_pipe" "lambda_method_pipe" 
			"local" "global" "in" ) 
		  'words) . font-lock-keyword-face)
    ;; cello functions
    (,(regexp-opt (list "lit" "cast" "with" "new" "delete" "allocate" "deallocate" "construct" 
			"destruct" "assign" "copy" "eq" "neq" "gt" "lt" "ge" "le" "len" "clear" 
			"contains" "discard" "is_empty" "sort" "maximum" "minimum" "reverse" 
			"iter_start" "iter_end" "iter_next" "hash" "push" "push_at" "push_back" 
			"push_front" "pop" "pop_at" "pop_back" "pop_front" "at" "set" "get" "put" 
			"as_char" "as_str" "as_long" "as_double" "enter_with" "exit_with" "open" 
			"close" "seek" "tell" "flush" "eof" "read" "write" "parse_read" "parse_write" 
			"type_class" "type_implements" "type_of" "add" "sub" "mul" "divide" "negate" 
			"absolute" "map" "new_map" "new_filter" "new_foldl" "new_foldr" "new_sum" 
			"new_product" "call_with" "call" "call_with_ptr" "release" "retain" "assert" 
			"format_to" "format_from" "format_to_va" "format_from_va" "show" "show_to" 
			"print" "print_to" "print_va" "print_to_va" "look" "look_from" "scan" 
			"scan_from" "scan_va" "scan_from_va" "println" "scanln" "current" "join" 
			"terminate" "lock" "unlock" "lock_try") 
		  'words) . font-lock-builtin-face))
    "A list of Cello keywords.")

(define-derived-mode cello-mode c-mode "Cello"
  (setcar font-lock-defaults (append cello-font-lock-keywords c-font-lock-keywords-3))
  (easy-menu-define cello-menu cello-mode-map "Cello Mode menu"
    '("Cello"
      :help "Cello-specific features"
      ["Cello browse doc at point" 'cello-browse-doc-at-point
      :help "Open the documentation for the thing at point."]
    )))

(provide 'cello-mode)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; cello-mode.el ends here

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; cello-mode.el ends here
