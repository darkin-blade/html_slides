


## 第一章

### 定理1.1 全概率公式

$A\_1,A\_2...A\_n$为完备事件组

$$P(B)=\sum\_{k=1}^nP(A\_k)P(B|A\_k)$$

### 定理1.2 贝叶斯公式

$A\_1,A\_2...A\_k$为完备事件组

$$P(A\_i|B)=\dfrac{P(A\_i)P(B|A\_i)}{\sum\_{k=1}^nP(A\_k)P(B|A\_k)}$$

### 定理1.6 泊松定理

设$n$为正整数,$\lambda=np\_n$为常数,则对任意正整数$k$有$$\lim\_{n\to\infty}C\_n^kp\_n^k(1-p\_n)^{n-k}=\dfrac{\lambda^k}{k!}e^{-\lambda}$$

## 第二章

### 常见离散型随机变量

- 0-1分布

- 二项分布

- 泊松分布

如果随机变量$X$的分布率为
$$p\_k=P(X=k)=\dfrac{\lambda^k}{k!}e^{-\lambda},k=0,1,2...$$
其中$\lambda>0$为常数,则称$X$服从参数为$\lambda$的泊松分布.记为$X\sim P(\lambda)$

- 几何分布

$$p\_k=P(X=k)=(1-p)^{k-1}p,k=1,2...$$

---

### 常见连续型随机变量

- 均匀分布

- 指数分布

若随机变量$X$的概率密度函数为
$$p(x)=\begin{cases}
\lambda e^{-\lambda x}&x\ge0\\\\
0&x < 0
\end{cases}$$
其中$\lambda > 0$为常数,则称$X$服从参数为$\lambda$的**指数分布**,记为$X\sim E(\lambda)$

$X$的分布函数为
$$
F(x)=\begin{cases}
1-e^{-\lambda x}&x\ge0\\\\
0&x < 0
\end{cases}
$$

- 正态分布

若随机变量$X$的密度函数为
$$p(x)=\dfrac{1}{\sqrt{2\pi}\sigma}e^{-\frac{(x-\mu)^2}{2\sigma^2}},-\infty < x < \infty$$

其中$\mu,\sigma(>0)$为常数,则称$X$服从参数为$\mu,\sigma^2$的正态分布,记为$X\sim N(\mu,\sigma^2)$

## 第三章

### 二维正态分布

$$p(x,y)=\dfrac{1}{2\pi\sigma\_1\sigma\_2\sqrt{1-\rho^2}}\exp\\{-\dfrac{1}{2(1-\rho^2)}[(\dfrac{x-\mu\_1}{\sigma\_1})^2-2\rho(\dfrac{x-\mu\_1}{\sigma\_1})(\dfrac{y-\mu\_2}{\sigma\_2})+(\dfrac{y-\mu\_2}{\sigma\_2})^2]\\}$$
$$\int\_0^{\infty}\dfrac{t^{x-1}}{e^t}dt=\Gamma(x)$$
$$\Gamma(x+1)=x\Gamma(x),\Gamma(\dfrac{1}{2})=\sqrt{\pi}$$

---

### 二维分布函数性质

- $F(x,y)$分别对每个变量单调不减
 - $\forall y,x\_2 > x\_1,F(x\_2,y) > F(x\_1,y)$
 - $\forall x,y\_2 > y\_1,F(x,y\_2) > F(x,y\_1)$

- $0\le F(x,y)\le1$,且
 - $\forall y,F(-\infty,y)=0$
 - $\forall x,F(x,-\infty)=0$
 - $F(-\infty,-\infty)=0$
 - $F(+\infty,+\infty)=1$

- $F(x,y)$关于每个变量右连续
 $$F(x,y)=F(x+0,y),F(x,y)=F(x,y+0)$$

- $F(x\_2,y\_2)-F(x\_2,y\_1)-F(x\_1,y\_2)+F(x\_1,y\_1)\ge0,(x\_2\ge x\_1,y\_2\ge y\_1)$


### 极大极小分布

- $F\_M(z)=P(\max\{X,Y\}\le z)=P(X\le z,Y\le z)$

- $F\_N(z)=P(\min\{X,Y\}\le z)=1-P(\min(X,Y)\ge z)=1-P(X > z,Y > z)$

当$X\_1,X\_2,...,X\_n$相互独立且同分布

- $F\_M(z)=[F(z)]^n$

- $F\_N(z)=1-[1-F(z)]^n$

### 二维随机变量函数的分布

如果随机变量$X$与$Y$相互独立,且$X\sim N(\mu\_1,\sigma\_1^2),Y\sim N(\mu\_2,\sigma\_2^2)$)

- $Z=X+Y\sim N(\mu\_1+\mu\_2,\sigma\_1^2+\sigma\_2^2)$
- $Z=aX+bY\sim N(a\mu\_1+b\mu\_2,a^2\sigma\_1^2+b^2\sigma\_2^2)$

推广:如果$X\_1,X\_2...X\_n$相互独立,$X\_i\sim N(\mu\_i,\sigma\_i^2)$

$$Z=a\_1X\_1+a\_2X\_2+...+a\_nX\_n\sim N(\sum\_{i=1}^ba\_i\mu\_i,\sum\_{i=1}^na\_i^2\sigma\_i^2)$$

---

## 第四章

### 定理4.1

若随机变量$X$的函数$Y=g(X)$也是一个随机变量,$g(X)$的数学期望存在...

- ...

- 连续型随机变量

$$EY=E[g(X)]=\int^{+\infty}\_{-\infty}g(x)p(x)dx$$

### 定理4.2

...$Z=g(X,Y)$

- ...

- 若$(X,Y)$为连续性的随机向量,其联合密度为$p(x,y)$,则

$$EZ=E[g(X,Y)]=\int^{\infty}\_{\infty}\int^{\infty}\_{\infty}g(x,y)p(x,y)dxdy$$


### 命题4.1

$$D(X)=EX^2-(EX)^2$$

### 切比雪夫不等式

对任意$\varepsilon > 0$

$$P(|X-EX|\ge\varepsilon)\le\dfrac{DX}{\varepsilon^2}$$

### 常见分布的期望与方差

| 记号 | 分布 | 参数 | 数学期望 | 方差 |
| :--: | :--: | :--: | :--: | :--: |
|  | 0-1分布 | $0 < p < 1$ | $p$ | $p(1-p)$ |
|  | 几何分布 | $0 < p < 1$ | $1/p$ | $\dfrac{1-p}{p^2}$ |
| $B(n,p)$ | 二项分布 | $n\ge1,0 < p < 1$ | $np$ | $np(1-p)$ |
| $P(\lambda)$ | 泊松分布 | $\lambda > 0$ | $\lambda$ | $\lambda$ |
| $U(a,b)$ | 均匀分布 | $a < b$ | $(a + b)/2$ | $(b-a)^2/12$ |
| $E(\lambda)$ | 指数分布 | $\lambda > 0$ | $1/\lambda$ | $1/\lambda^2$ |
| $N(\mu,\sigma^2)$ | 正态分布 | $\mu\in R,\sigma > 0$ | $\mu$ | $\sigma^2$ |

---
### 协方差,cov$(X,Y)$

$$\text{cov}(X,Y)=E[(X-EX)(Y-EY)]=E(XY)-E(X)E(Y)$$
$$D(X\pm Y)=D(X)+D(Y)\pm2\text{cov}(X,Y)$$

### 相关系数

设随机变量的二阶矩有限,$D(X) > 0,D(Y) > 0$,则
$$\dfrac{\text{cov}(X,Y)}{\sqrt{D(X)D(Y)}}$$
为**$X$和$Y$的相关系数**,记为$\rho\_{XY}$或者corr$(X,Y)$


## 第五章

### 定义 5.1 依概率收敛

设$X\_1,X\_2...X\_n...$为一列随机变量,若存在随机变量$X$,使得任意给定的$\varepsilon > 0$,$\lim\_{n\to\infty}P(|X\_n-X|\ge\varepsilon)=0$或等价地$\lim\_{n\to\infty}P(|X-X\_n|) < \varepsilon)=1$,则称**随机变量序列**$\\{X\_n\\}$**依概率收敛于**随机变量$X$,记为$X\_n\overset{P}{\to}X$

### 定义 5.2 大数定律

对随机变量序列$\\{X\_n\\}$,若任意的$\varepsilon > 0$,有
$$ \lim\_{n\to\infty}P(|\dfrac{1}{n}\sum\_{k=1}^{n}X\_k-\dfrac{1}{n}\sum\_{k=1}^{n}EX\_k|\ge\varepsilon)=0 $$
或
$$ \lim\_{n\to\infty}P(|\dfrac{1}{n}\sum\_{k=1}^{n}X\_k-\dfrac{1}{n}\sum\_{k=1}^{n}EX\_k| < \varepsilon)=1 $$
则称$\\{X\_n\\}$**服从大数定律**

### 定理 5.1 切比雪夫大数定律

设$\\{X\_n\\}$为两两互不相关的随机变量序列,又存在常数$C > 0$,使得对每个随机变量$X\_k,D(X\_k)\le C,k=1,2...$则$\\{X\_n\\}$服从大数定律

---
### 定理 5.2

若$\\{X\_n\\}$为一列独立同分布的随机变量,设其满足$EX\_n=\mu,D(X\_n)=\sigma^2 < \infty$.则$\\{X\_n\\}$服从大数定律,即$\dfrac{1}{n}\sum\_{k=1}^{n}X\_k\overset{P}{\to}\mu(=EX\_k)$

### 定理 5.3 Bernoulli大数定律

设$\mu\_n$为$n$重Bernoulli试验中事件$A$发生的次数,$p$为事件$A$在每次试验中发生的概率,则任意的$\varepsilon > 0$
$$ \lim\_{n\to\infty}P(|\dfrac{\mu\_n}{n}-p|\ge\varepsilon)=0 $$

### 定理 5.4 列维-林德伯格中心极限定理

设$\\{X\_n\\}$为独立同分布的随机变量序列,$EX\_n=\mu,D(X\_n)=\sigma^2$都存在,则任意的$x\in(-\infty,+\infty)$一致地有
$$ \lim\_{n\to\infty}P(\dfrac{\sum\_{k=1}^nX\_k-n\mu}{\sqrt{n\sigma^2}}\le x)=\dfrac{1}{\sqrt{2\pi}}\int\_{-\infty}^{x}e^{-\frac{t^2}{2}}dt=\Phi(x) $$

