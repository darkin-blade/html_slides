## Sage准备

- mac
 1. 开启共享
 2. `Applications/SageMath-8.6.app/Contents/Resources/sage/sage`

### 模$n$整数$Z\_n$


```python
Z8 = Integers(8)
Z8.list()
# [0, 1, 2, 3, 4, 5, 6, 7]

a = Z8.an_element(); a
# 0

a = Z8(7); a
# 7

Z8.addition_table(names = 'elements')
# +  0 1 2 3 4 5 6 7
#  +----------------
# 0| 0 1 2 3 4 5 6 7
# 1| 1 2 3 4 5 6 7 0
# 2| 2 3 4 5 6 7 0 1
# 3| 3 4 5 6 7 0 1 2
# 4| 4 5 6 7 0 1 2 3
# 5| 5 6 7 0 1 2 3 4
# 6| 6 7 0 1 2 3 4 5
# 7| 7 0 1 2 3 4 5 6
```

### 子群subgroups

- 群
 - 结合律
 - 单位元存在
 - 逆元存在

- 子群
 - 封闭性
 - 单位元存在性
 - 逆元存在性


```python
S8 = SymmetricGroup(8)
Q = QuaternionGroup()
Q.is_subgroup(S8)
# True

a = S8.random_element()
[a (x) for x in S8.domain()]
# [5, 2, 6, 4, 1, 8, 3, 7]
```

### 一般线性群

- $\mathbb{M}\_2(\mathbb{R})$:所有$2\times2$矩阵
- $GL\_2(\mathbb{R})$:$\mathbb{M}\_2$中可逆矩阵.可逆矩阵构成一般线性群
- $A\in GL\_2(\mathbb{R}),A^{-1}=\dfrac{1}{ad-bc}
\begin{pmatrix}
d & -b \\\\
-c & a \\\\
\end{pmatrix}
$

```python
matrix([[1,0],[3,4]])
# [1 0]
# [3 4]
```

### 循环群

- 存在一些$a\in G$使得$G=\langle a\rangle$
- $C\_{\infty}\cong\mathbb{Z}(\mathbb{Z},+),C\_n\cong\mathbb{Z}\_n$
- 无限循环群$C\_{\infty}$
 - $|g^k|=\infty$
 - 生成元:$g,g^{-1}$
 - 子群:$\langle g^k\rangle,k\in\mathbb{N}$
- 有限循环群$C\_n$
 - 生成元:$g^k,\gcd(k,n)=1$

```python
G = 3*ZZ
-12 in G
# True
G.gen()
# 3
```

### 四元群$Q\_8$

- $Q\_8=\{\pm1,\pm I,\pm J,\pm K\}$,其中

$$
1=
\begin{pmatrix} 
1 & 0 \\\\
0 & 1 \\\\
\end{pmatrix} 
I=
\begin{pmatrix} 
0 & 1 \\\\
-1 & 0 \\\\
\end{pmatrix} 
J=
\begin{pmatrix} 
0 & i \\\\
i & 0 \\\\
\end{pmatrix} 
K=\begin{pmatrix} 
i & 0 \\\\ 
0 & -i \\\\
\end{pmatrix}
$$

```python
Q = QuaternionGroup()
Q.order()
# 8
Q.is_abelian()
# False
```

> Sage中复数用`CC`表示,可用`i`/`I`

```python
H = [CC(1), CC(-1), CC(i), CC(-i)]
sage: CC.multiplication_table(elements = H, names = ['1', '-1', 'i', '-i'])
#  *   1 -1  i -i
#   +------------
#  1|  1 -1  i -i
# -1| -1  1 -i  i
#  i|  i -i -1  1
# -i| -i  i  1 -1
```

### 二面体群(Dihedral Group)$D\_n$

```python
D100 = DihedralGroup(100)
D50 = DihedralGroup(50)
Z2 = Integers(2)
D100.is_abelian()
# False
D100.is_cyclic()
# False
```

### 置换群$S\_n$:

- 秩:$|S\_n|=n!$

```python
S3 = SymmetricGroup(3)
rho1 = S3([3, 2, 1]); rho1
# (1,3)

S3.domain()
# {1, 2, 3}

[rho1(x) for x in S3.domain()]
# [3, 2, 1]

[[a(x) for x in S3.domain()] for a in S3]
# [[1, 2, 3], [3, 1, 2], [2, 3, 1], [1, 3, 2], [3, 2, 1], [2, 1, 3]]

S3.cayley_table(names='elements')
#       *       ()   (2,3)   (1,2) (1,2,3) (1,3,2)   (1,3)
#        +------------------------------------------------
#      ()|      ()   (2,3)   (1,2) (1,2,3) (1,3,2)   (1,3)
#   (2,3)|   (2,3)      () (1,2,3)   (1,2)   (1,3) (1,3,2)
#   (1,2)|   (1,2) (1,3,2)      ()   (1,3)   (2,3) (1,2,3)
# (1,2,3)| (1,2,3)   (1,3)   (2,3) (1,3,2)      ()   (1,2)
# (1,3,2)| (1,3,2)   (1,2)   (1,3)      () (1,2,3)   (2,3)
#   (1,3)|   (1,3) (1,2,3) (1,3,2)   (2,3)   (1,2)      ()
```

```python
S5 = SymmetricGroup(5)
sigma = S5("(1,3)(2,5,4)")
sigma**2
# (2,4,5)
```

#### 循环置换群

```python
C3 = CyclicPermutationGroup(3)
```

### 交代群$A\_n$

- $S\_n$的子群,偶排列,$|A\_n|=\dfrac{n!}{2}$

> 如果排列为偶`.sign()`输出1,为奇输出-1

```python
A4 = AlternatingGroup(4)
rho = A4.random_element()
rho
# (1,4,2)

rho.sign()
# 1

S4 = SymmetricGroup(4)
rho = S4.random_element()
rho; rho.sign()
# (1,4,2,3)
# -1

A4.order()' S4.order()
# 12
# 24
```

> 通过给定generators来生成一个群的子群

```python
sigma = (1,4,2)
sg = A4.subgroup([sigma])
sg.list()
# [(), (1,2,4), (1,4,2)]
```

> 穷举所有子群(注意`s`)

```python
A4.subgroups()
```

### 陪集

- $H$是$G$的子群,$H$的左陪集:$gH=\\{gh:h\in H\\}$

```python
S3 = SymmetricGroup(3)
a = S3("(1,2)")
H = S3.subgroup([a])
rc = S3.cosets(H, side = 'right'); rc
# [[(), (1,2)], [(2,3), (1,3,2)], [(1,2,3), (1,3)]]

lc = S3.cosets(H, side = 'left'); lc
# [[(), (1,2)], [(2,3), (1,2,3)], [(1,3,2), (1,3)]]
```

> 乱序的list比较结果为False,可通过排序再进行比较

```python
b = S3("(1,2,3)")
H = S3.subgroup([b])
rc = S3.cosets(H, side = 'right');
lc = S3.cosets(H, side = 'left');
rc == lc
# False

rc_sorted = sorted([sorted(coset) for coset in rc])
lc_sorted = sorted([sorted(coset) for coset in lc])
rc_sorted == lc_sorted
# True
```

- $H$是$G$的子群,index of $H=H$在$G$中左陪集的个数$=H$右陪集的个数,记作$[G:H]$

### 正规子群(Normal Subgroups)

- $H$是$G$的子群,$gH=Hg,\forall g\in G$
- 等价条件,设$N$是$G$的子群
 - $N$是$G$的正规子群
 - $\forall g\in G,gNg^{-1}\subset N$
 - $\forall g\in G,gNg^{-1}=N$

```python
A4.is_normal(S4)
# True
```

### 商群(factor/quotient group)

- $N$是$G$的正规子群,商群$G/N$是$N$在$G$中左陪集的集合
- $aN,bN\in G/N,(aN)(bN)=abN$
- $G/N$的阶为$[G:N]$

```python
Q = S4.quotient(A4)
```

### 同构 同态

- 同构:$(G,\cdot),(H,\circ)$单射且满射$\phi:G\to H$

$$\phi(a\cdot b)=\phi(a)\circ\phi(b),\forall a,b\in G$$

- 同态:$(G,\cdot),(H,\circ)$映射$\phi:G\to H$

$$\phi(a\cdot b)=\phi(a)\circ\phi(b),\forall a,b\in G$$

- 核(kernel):群同态$\phi:G\to H$,$e$是$H$的单位元,$G$的子群$\phi^{-1}(\{e\})$是$\phi$的kernel

```python
A4.is_isomorphic(S4)
# False

x = S4.gen(0)
phi = PermutationGroupMorphism(A4, S4, x)
phi
# Permutation group morphism:
#   From: Alternating group of order 4!/2 as a permutation group
#   To:   Symmetric group of order 4! as a permutation group
#   Defn: [(2,3,4), (1,2,3)] -> [(1,2,3,4)]

phi = PermutationGroupMorphism(A4, S4, A4.gens()); phi
# Permutation group morphism:
#   From: Alternating group of order 4!/2 as a permutation group
#   To:   Symmetric group of order 4! as a permutation group
#   Defn: [(2,3,4), (1,2,3)] -> [(2,3,4), (1,2,3)]

a = A4.random_element(); print(a, "->", phi(a))
# ((2,3,4), '->', (2,3,4))

phi.kernel()
# Subgroup of (Alternating group of order 4!/2 as a permutation group) generated by [()]
```

### null space

`sage`中null space使用kernel代替

```python
Z = Integers(2)
Z
# Ring of integers modulo 2
M = matrix(Z,[[0,1,0,0,0],[1,0,1,0,1],[1,0,0,1,0]])
M
# [0 1 0 0 0]
# [1 0 1 0 1]
# [1 0 0 1 0]
H = M.right_kernel_matrix()
H
# [1 0 0 1 1]
# [0 0 1 0 1]
```

