# HuiDong3D 渲染库


**环境**

easyx 2021-7-30 + VS2019

---

**简介**

由于本人暂时只有初中数学知识……自学 3D 图形学理论然后开发了这个渲染库。

这个渲染库还欠缺很多功能，一起努力学习吧！

---

**效果**

![人脸模型](https://github.com/zouhuidong/HuiDong3D/blob/master/screenshots/2.png)
![人脸模型](https://github.com/zouhuidong/HuiDong3D/blob/master/screenshots/3.png)
![立方体](https://github.com/zouhuidong/HuiDong3D/blob/master/screenshots/1.png)


---

**功能列表**

- [x] 3D 信息存储
- [x] 3D 旋转运算
- [x] 多边形网格
- [x] 平行投影渲染
- [x] 透视投影渲染（有一点点小bug）
- [x] 视口裁剪（但是目前只是很简单的裁剪，以后更新）
- [x] 创建多个 3D 物体
- [x] 创建多个 3D 场景
- [x] 摄像机自定义调节
- [ ] UV 纹理
- [ ] amp 并行计算
- [ ] 光线追踪

---

**已知bug**

透视渲染时如果物体距离摄像机比较远，会出现扭曲。

---

**目标**

以后还要再继续完善视口裁剪功能，实现 UV 贴图。如果以后有时间的话，可以尝试使用 amp 进行计算。

对了，帧率也要优化一下，现在的帧率有点寒酸……

但是……马上要开学了，不知道以后还有没有时间了，希望我可以继续把这个渲染库完善下去。

