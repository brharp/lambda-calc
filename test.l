;; Conses. A cons is a closure that binds 2 variables:
;; the car and the cdr (x and y). The cons closure is
;; a function that takes one argument (a selector) and
;; applies the selector to the bound variables. The 
;; selector 'getcar' accepts x and y and returns x, the
;; selector 'getcdr' accepts x and y and returns y. The
;; functions 'car' and 'cdr' are convenience functions
;; that apply a cons to the appropriate selector.
cons = \x.\y.\f.f x y
getcar = \x.\y.x
getcdr = \x.\y.y
car = \x.x getcar
cdr = \x.x getcdr
caar = \x.car (car x)
cadr = \x.car (cdr x)
cdar = \x.cdr (car x)
cddr = \x.cdr (cdr x)
;; The evaluator used to allow modifying values,
;; which made the following definitions of rplaca
;; and rplacd possible. Values are now immutable.
;rplaca = \x.\y.(cons y (cdr x))
;rplacd = \x.\y.(cons (car x) y)
q = cons 'i 'j
car q
cdr q
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
if = \pred.\cons.\ante.pred cons ante
true = \cons.\ante.cons
false = \cons.\ante.ante
if true 'yes 'no
if false 'yes 'no
;; Numbers.
plus = \m.\n.\f.\x.m f (n f x)
succ = \n.\f.\x.f (n f x)
mult = \m.\n.\f.m (n f)
;; Predecessor.
pred = \n.\f.\x.n (\g.\h.h (g f)) (\u.x) (\u.u)
;; How pred works. Take for example "pred 3".
;; Apply "\g.\h.h (g f)" 3 times:
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

0 = \f.\x.x
1 = \f.\x.f x
2 = succ 1
3 = succ 2
4 = succ 3
5 = succ 4
6 = succ 5
7 = succ 6
8 = succ 7
9 = succ 8

2 print 'x

f = \y.cons 'x y
g = f 'nil
car g
cdr g
h = 1 f 'nil
car h
cdr h
i = 2 f 'nil
car i
car (cdr i)
cadr i
cddr i

j = pred 2 f 'nil
car j
cdr j

zerop = \n.n (\x.false) true

fact = \n.if (zerop n) 1 (mult (fact (pred n)) n)
(fact 4) print 'x


