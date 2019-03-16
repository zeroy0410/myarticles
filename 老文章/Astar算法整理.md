---
title: Astar算法整理
password: ZlOi
---

#### 前言

> A*算法，A*（A-Star)算法是一种静态路网中求解[最短路径](https://baike.baidu.com/item/%E6%9C%80%E7%9F%AD%E8%B7%AF%E5%BE%84/6334920)最有效的**直接**搜索方法，也是解决许多搜索问题的[有效](https://baike.baidu.com/item/%E6%9C%89%E6%95%88)算法。算法中的[距离](https://baike.baidu.com/item/%E8%B7%9D%E7%A6%BB/16370594)估算值与实际值越接近，最终[搜索](https://baike.baidu.com/item/%E6%90%9C%E7%B4%A2/1806)速度越快。(百度百科)

简单来说，就是把原来bfs的FIFO队列改为现在的优先队列，通过估价函数$H()$来实时弹出当前堆中的最优解，让答案最快地被找到。

堆中的比较一般这样写：

```c++
bool operator < (const node& res) const{
    return step+H>res.step+res.H;
}
```



由于手动的添加了一个log，所以最终的效果很大程度上取决于估价函数，如果估价函数写得不好，~~反而有可能变慢~~.

#### 例题（YCJSOI2963 贪吃蛇）

[内网题目链接](http://10.220.121.203/judge/problem.php?id=2963)

这题的其它方面不再赘述。主要讲讲A*的估价函数。

这题里的估价函数很好设计，从终点直接bfs即可。

~~Astar格式比较固定，但具体题目有具体的方法，没有明显规律，我实在编不下去了~~