# 2

## 2.3

### Definition 2.3.1.2

Let $\Sigma$ be an alphabet. A **word** over $\Sigma$ is any finite sequence of symbols of $\Sigma$. The **empty word $\lambda$** is the only word consisting of zero symbols. The set of all words over the alphabet $\Sigma$ is denoted by $\Sigma^{\*}$

### Definition 2.3.1.3

The **length of a word $w$** over an alphabet $\Sigma$, denoted by $|w|$, is the number of symbols in $w$. For every word $w\in\Sigma^{\*}$, and every symbal $a\in\Sigma$, $\sharp\_{a}(w)$ is the number of occurrences of the symbol $a$ in the word $w$

### Definition 2.3.1.4

Let $\Sigma$ be an alphabet. Then, for any $n\in\mathbb{N}$
$$ \Sigma^n=\\{x\in\Sigma^{\*}||x|=n\\} $$
We define $\Sigma^+=\Sigma^{\*}-\\{\lambda\\}$

### Definition 2.3.1.5 (concatenation)

For every word $w\in\Sigma^{\*}$, we define

- $w^0=\lambda$, and
- $w^{n + 1}=w\cdot w^n=ww^n$ for every positive integer $n$

### Definition 2.3.1.9 (language)

Let $\Sigma$ be an alphabet, Every set $L\subseteq\Sigma^{\*}$ is called a **language** over $\Sigma$. The **complement of the language $L$ according to $\Sigma$** is $L^C=\Sigma^{\*}-L$

Let $\Sigma\_1$ and $\Sigma\_2$ be alphabets, and let $L\_1\subseteq\Sigma\_1^{\*}$ and $L\_2\subseteq\Sigma\_2^{\*}$ be languages. The **concatenation** of $L\_1$ and $L\_2$ is
$$ L\_1L\_2=L\_1\circ L\_2=\\{uv\in(\Sigma\_1\cup\Sigma\_2)^{\*}|u\in L\_1 and\ v\in L\_2\\} $$

---

### Definition 2.3.1.10

Let $\Sigma=\\{s\_1,s\_2...s\_m\\},m\ge1$, be an alphabet, and let $s\_1 < s\_2 < ... < s\_m$ be a linear ordering on $\Sigma$. We define the **canonical ordering** on $\Sigma^{\*}$ as follows. For all $u,v\in\Sigma^{\*}$
$$u < v\mbox{ if }|u| < |v|$$
$$\mbox{or }|u|=|v|,u=xs\_iu',\mbox{ and }v=xs\_jv'$$
$$\mbox{for some }x,u',v'\in\Sigma^{\*},\mbox{ and }i < j$$

## 2.3.2

### Definition 2.3.2.1

**A decision problem** is a triple $(L, U, \Sigma)$ where $\Sigma$ is an alphabet and $L\subseteq U\subseteq\Sigma^{\*}$. An algorithm $A$ **solves(decides)** the decision problem $(L, U, \Sigma^{\*}$ if, for every $x\in U$

- $A(x)=1$ if $x\in L$, and
- $A(x)=0$ if $x\in U-L(x\not\in L)$

### Definition 2.3.2.2

An **optimization problem** is a 7-tuple $U=(\Sigma\_I,\Sigma\_O,L,L\_I,M,cost,goal)$, where

- $\Sigma\_I$ is an alphabet, called the **input alphabet** of $U$
- $\Sigma\_O$ is an alphabet, called the **output alphabet** of $U$
- $L\subseteq\Sigma\_I^{\*}$ is the **language of feasible problem instances**
- $L\_I\subseteq L$ is the **language of the(actual) problem instances of $U$**
- $M$ is a function from $L$ to (power set)$Pot(\Sigma\_O^{\*})$, and for every $x\in L$, $M(x)$ is some $x\in L$, assigns a positive real number cost($u,x$)
- cost is the **cost function** that, for every pair $(u,x)$, where $u\in M(x)$ for some $x\in L$, assigns a positive real number cost($u,x$)
- goal$\in$\{minimum,maximum\}

For every $x\in L\_I$, a feasible solution $y\in M(x)$ is called **optimal for $x$ and $U$** if
$$ cost(y,x)=goal\\{cost(z,x)|z\in M(x)\\} $$
For an optimal solution $y\in M(x)$, we denote cost($u,x$) by Opt$\_U(x)$ ,$U$ is called **maxmization problem** if goal=maximum, and $U$ is a **minimization problem** if goal=minimum. In what follows Output$\_U(x)\subseteq M(x)$ denotes the set of all optimal solutions for the instance $x$ of $U$

An algorithm $A$ is **consistent** for $U$ if, for every $x\in L\_I$, the output $A(x)\in M(x)$. We say that an algorithm $B$ **solves** the optimization problem $U$ if

- $B$ is consistent for $U$, and
- for every $x\in L\_1, B(x)$ is an optimal solution for $x$ and $U$

---

### Definition 2.3.2.3

Let $U\_1=(\Sigma\_I,\Sigma\_O,L,L\_{I,1},M,cost,goal)$ and $U\_2=(\Sigma\_I,\Sigma\_O,L,L\_{I,2},M,cost,goal)$ be two optimization problems. We say that $U\_1$ is a **subproblem** of $U\_2$ if $L\_{I,1}\subseteq L\_{I,2}$

### Theorem 2.3.3.3

There is a decision problem $(L,\Sigma\_{bool})$ such that, for every algorithm $A$ deciding $L$, there exists another algorithm $B$ deciding $L$, such that
$$ Time\_B(n)=\log\_2(Time\_A(n)) $$
for infinitely many positive integers $n$

### Definition 2.3.3.5

We define the complexity class $P$ of languages decidable in polynomial-time by
$$ P=\\{L=L(M)|M\mbox{ is a TM(an algorithm) with }Time\_M(n)\in O(n^c)\mbox{ for some positive integer c}\\} $$

A language (decision problem) $L$ is called **tractable(practially solvable)** if $L\in P$. A language $L$ is called **intractable** if $L\not\in P$

### Definition 2.3.3.6

Let $M$ be a nondeterministic TM(algorithm).  We say that **$M$ accepts a language $L, L = L(M)$**, if

- for every $x\in L$, there exists at least one computation of $M$ that accepts $x$, and
- for every $y\not\in L$, all computations of $M$ reject $y$

For every input $w\in L$, the ** time complexity Time$\_M(w)$ of $M$ on $w$ ** is the time complexity of the shortest accepting computation of $M$ on $w$. The **time complexity of $M$** is the function Time$\_M$ from $\mathbb{N}$ to $\mathbb{N}$ defined by
$$ Time\_M(n)=\max\\{Time\_M(x)|x\in L(M)\cap\Sigma^n\\} $$

---

### Definition 2.3.3.7

Let $L\subseteq\Sigma^{\*}$ be a language. An algorithm $A$ working on inputs from $\Sigma^{\*}\times\\{0,1\\}$ is called a **verifier for $L$**, denoted $L=V(A)$, if
$$ L=\\{w\in\Sigma^{\*}|A\mbox{ accepts }(w,c)\mbox{ for some }c\in\\{0,1\\}^{\*}\\} $$
If $A$ accepts $(w,c)\in\Sigma^{\*}\times\\{0,1\\}$, we say that $c$ is a **proof(certificate)** of the fact $w\in L$

A verifier $A$ for $L$ is called a **polynomial-time verifier** if there exists a positive integer $d$ such that, for every $w\in L$, Time$\_A(w,c)\in O(|w|^d)$ for a proof $c$ of $w\in L$

We define the **class of polynomially verifiable languages** as
$$ VP=\\{V(A)|A\mbox{ is a polynomial-time verifier}\\} $$

### Definition 2.3.3.10

Let $L\_1\subseteq\Sigma\_1^{\*}$ and $L\_2\subseteq\Sigma\_2^{\*}$ be two languages. We say that $L\_1$ is **polynomial-time reducible** to $L\_2, L\_1\le\_pL\_2$, if there exists a polynomial-time algorithm $A$ that computes a mapping from $\Sigma\_1^{\*}$ to $\Sigma\_2^{\*}$ such that, for every $x\in\Sigma\_1^{\*}$
$$ x\in L\_1\Leftrightarrow A(x)\in L\_2 $$
A is called the **polynomial-time reduction** from $L\_1$ to $L\_2$
- A language $L$ is called **NP-hard** if, for every $U\in NP,U\le\_pL$
- A language $L$ is called **NP-complete** if
 - $L\in NP$, and
 - $L$ is NP-hard

### Lemma 2.3.3.11

If $L$ is NP-hard and $L\in P$, then P=NP

### Lemma 2.3.3.15

Sat$\le\_{p}$Clique

### Lemma 2.3.3.16

Clique$\le\_{p}$VC

### Lemma 2.3.3.19

3Sat$\le\_{p}$Sol-0/1-LP

---

### Definition 2.3.3.21

**NPO** is the class of optimization problems, where $U=(\Sigma\_I,\Sigma\_O,L,L\_I,M,cost,goal)\in$NPO if the following conditions hold
- $L\_I\in$P
- there exists a polynomial $p\_U$ such that
 - for every $x\in L\_1$, and every $y\in M(x),|y|\le p\_U(|x|)$, and
 - there exists a polynomial-time algorithm that, for every $y\in\Sigma\_O^{\*}$ and every $x\in L\_1$ such that $|y|\le p\_U(|x|)$, decides whether $y\in M(x)$, and
 - the function cost is computable in polynomial time

### Definition 2.3.3.23

**PO** is the class of optimization problems $U=(\Sigma\_I,\Sigma\_O,L,L\_I,M,cost,goal)$ such that
- $U\in$NPO, and
- there is a polynomial-time algorithm that, for every $x\in L\_1$, computes an optimal solution for $x$

### Definition 2.3.3.24

Let $U=(\Sigma\_1,\Sigma\_O,L,L\_1,M,cost,goal)$ be an optimization problem from NPO, We define the **threshold language of $U$** as
$$ Lang\_U=\\{(x,a)\in L\_1\times\Sigma\_{bool}^{\*}|Opt\_U(x)\le Number(a)\\} $$
if goal=minimum, and as
$$ Lang\_U=\\{(x,a)\in L\_1\times\Sigma\_{bool}^{\*}|Opt\_U(x)\ge Number(a)\\} $$
if goal=maximum

We say that $U$ is **NP-hard** if Lang$\_U$ is NP-hard

### Lemma 2.3.3.25

If an optimization problem $U\in$PO, then Lang$\_U\in$P

---

# 4

## 4.2

### Definition 4.2.1.1

Let $U=(\Sigma\_I,\Sigma\_O,L,L\_I,M,cost,goal)$ be an optimization problem, and let $A$ be a consistent algorithm for $U$. For every $x\in L\_I$, the **relative error** $\varepsilon\_A(x)$ **of $A$ on $x$** is defined as
$$ \varepsilon\_A=\dfrac{|cost(A(x))-Opt\_U(x)|}{Opt\_U(x)} $$

- For any $n\in\mathbb{N}$, we define **the relative error of $A$** as
$$ \varepsilon\_A(n)=\max\\{\varepsilon\_A(x)|x\in L\_I\cap(\Sigma\_I)^n\\} $$

- For every $s\in L\_I$, the **approximation ratio** $R\_A(x)$ **of $A$ on $x$** is defined as
$$ R\_A(x)=\max\\{\dfrac{cst(A(x))}{Opt\_U(x)},\dfrac{Opt\_U(x)}{cost(A(x))}\\} $$ 

- For any $n\in\mathbb{N}$, we define the **approximation ratio of $A$** as
$$ R\_A(n)=\max\\{R\_A(x)|x\in L\_I\cap(\Sigma\_I)^n\\} $$

- For any positive real $\delta > 1$, we say that $A$ is a **$\delta$-approximation algorithm for $U$** if $R\_A(x)\le\delta$ for every $x\in L\_I$

- For every function $f:\mathbb{N}\to\mathbb{R}^+$, we say that $A$ si an **$f(n)$-approximation algorithm for $U$** if $R\_A(n)\le f(n)$ for every $n\in\mathbb{N}$

### Definition 4.2.1.6

Let $U=(\Sigma\_I,\Sigma\_O,L,L\_I,M,cost,goal)$ be an optimization problem. An algorithm $A$ is called a **polynomial-time approximation scheme (PTAS) for $U$** if for every input pair $(x,\varepsilon)\in L\_I\times\mathbb{R}^+, A$ computes a feasible solution $A(x)$ with a relative error at most $\varepsilon$, and $Time\_A(x,\varepsilon^{-1})$ can be bounded by a function that is polynomial in $|x|$.

If $Time\_A(x,\varepsilon^{-1})$ can be bounded by a function that is polynomial in both $|x|$ and $\varepsilon^{-1}$, then we say that $A$ is a **fully polynomial-time approximation scheme (FPTAS) for $U$**

---

### Definition 4.2.3.1

Let $U=(\Sigma\_I,\Sigma\_O,L,L\_I,M,cost,goal)$ and $\bar{U}=(\Sigma\_I,\Sigma\_O,L,L,M,cost,goal)$ be two optimization problems with $L\_I\subset L$. A **distance function for $\bar{U}$ according to** $L\_I$ is any function $h\_L:L\to\mathbb{R}^{\ge0}$ satisfying the properties

1. $h\_L(x)=0$ for every $x\in L\_I$, and
2. $h$ is polynomial-time computable

Let $h$ be a distance function for $\bar{U}$ accoring to $L\_I$. We define, for any $r\in\mathbb{R}^+$
$$ Ball\_{r,h}(L\_I)=\\{w\in L|h(w)\le r\\} $$
Let $A$ be a consistent algorithm for $\bar{U}$, and let $A$ be an $\varepsilon$-approximation algorithm for $U$ for some $\varepsilon\in\mathbb{R}^{-1}$. Let $p$ be a positive real. We say that $A$ is **p-stable according to $h$** if, for every real $0 < r \le p$, there exists a $\delta\_{r,\varepsilon}\in\mathbb{R}^{> 1}$ such aht $A$ is a $\delta{r,\varepsilon}$-approximation algorithm for $U\_r=(\Sigma\_I,\Sigma\_O,L,Ball\_{r,h}(L\_I),M,cost,goal)$

