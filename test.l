;; Conses. A cons is a closure that binds 2 variables:
;; the car and the cdr (x and y). The cons closure is
;; a function that takes one argument (a selector) and
;; applies the selector to the bound variables. The 
;; selector 'getcar' accepts x and y and returns x, the
;; selector 'getcdr' accepts x and y and returns y. The
;; functions 'car' and 'cdr' are convenience functions
;; that apply a cons to the appropriate selector.
cons = \x.\y.\f.f x y
;; cons = function (x) { return function (y) { return function (f) { return f(x,y); } } }
getcar = \x.\y.x
;; getcar = function (x) { return function (y) { return x; } }
getcdr = \x.\y.y
car = \x.x getcar
cdr = \x.x getcdr
caar = \x.car (car x)
cadr = \x.car (cdr x)
cdar = \x.cdr (car x)
cddr = \x.cdr (cdr x)
;rplaca = \x.\y.(cons y (cdr x))
;rplacd = \x.\y.(cons (car x) y)
q = cons 'i 'j
car q
cdr q
;rplaca q 'k
;car q
;; Infinite lists. Due to the laziness of the evaluator,
;; infinite structures can be defined. The function 'inf'
;; creates an inifinite list of its argument:
inf = \x.cons x (inf x)
(inf 'x)
(car (inf 'x))
(cdr (inf 'x))
(car (cdr (inf 'x)))
(car (cdr (cdr (inf 'x))))
;; Conditionals. Conditional statements can be defined in
;; lambda calculus with no special support from the 
;; evaluator. The conditional statement 'if' is a function
;; of 3 arguments: the predicate, the consequent, and the
;; antecedent. If the predicate is true, 'if' evaluates
;; the consequent; if the predicate is false, 'if' evaluates
;; the antecedent. The boolean values true and false are
;; themselves functions: true is a function of 2 arguments
;; (a consequent and an antecedent) that evaluates its
;; consequent; false is like true but evaluates the antecedent.
if = \predicate.\consequent.\antecedent.predicate consequent antecedent
true = \consequent.\antecedent.consequent
false = \consequent.\antecedent.antecedent
if true 'yes 'no
if false 'yes 'no
;; Numbers.
plus = \m.\n.\f.\x.m f (n f x)
succ = \n.\f.\x.f (n f x)

;; Predecessor.
pred = \n.\f.\x.n (\g.\h.h (g f)) (\u.x) (\u.u)
;; How pred works.
;; Apply "\g.\h.h (g f)" n times:
;; (\g.\h.h (\g.\h.h (\g.\h.h (g f)) f) f) (\u.x) (\u.u)
;; You will notice 3 applications of 'h' and only one of 'g'.
;; The 'g' is bound to (\u.x) and the 'h' is bound to (\u.u):
;; (\u.u (\u.u (\u.u (\u.x f)) f) f)
;; Working from the inside out, the desired result
;; is obtained:
;; (\u.u (\u.u (\u.u x) f) f)
;; (\u.u (\u.u x) f) f)
;; (\u.u (f x) f)
;; (f (f x))

one = \f.\x.f x
two = plus one one
f = \y.cons 'x y
g = f 'nil
car g
cdr g
h = one f 'nil
car h
cdr h
i = two f 'nil
car i
car (cdr i)
cadr i
cddr i

j = pred two f 'nil
car j
cdr j

