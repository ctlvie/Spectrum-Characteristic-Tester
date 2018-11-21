# 使用 PSpice 仿真

## 导入电路元件模型

1. 在器件供应商网站下载其 PSpice 模型
2. 用 PSpice Model Editor 打开模型
3. 另存为 .lib 格式
4. 导出为 .olb 格式
5. 在 OrCAD Capture 中导入元件模型
6. 注意导入元件的管脚定义,往往和器件的管脚是不一致的,需要参考原始的模型文件中的定义

```txt
    例如: OP-07C在其 PSpice 中的管脚定义如下
    * CONNECTIONS:   NON-INVERTING INPUT
    *                | INVERTING INPUT
    *                | | POSITIVE POWER SUPPLY
    *                | | | NEGATIVE POWER SUPPLY
    *                | | | | OUTPUT
    *                | | | | |
    .SUBCKT OP-07C   1 2 3 4 5
```

    Tips: NON-INVERTING INPUT 代表 IN+; INVERTING INPUT 代表 IN-;

## 新建 OrCAD Capture 工程

1. File -> New -> Project -> PSpice Analog or Mixed AD
2. 选择基于“simple.opj”为模板创建，这种模板自带常用的元件库

## 绘制电路原理图

1. 在放置元件时可先选择 Libararies,按下 Ctrl+A 全选后,在输入具体器件型号查找
2. 常用快捷键: 需要打开大写
    * P : 放置元件
    * W : 放置线
    * G : 放置地
    * F : 放置电源
    * Esc : 结束放置
3. 信号源的放置: Shift+R 快捷键打开 Independent Sources 面板,设置信号源的各种参数后点击 Place 即可放置

## 直流工作点分析

1. 通过顶部工具栏中的 PSpice -> New Simulation Profile 对电路进行仿真设置,默认情况下即为直流工作点分析
2. 在仿真设置界面中的 Configuration Files 中将自己添加元件的 .lib 库文件加入到仿真中, 否则后续仿真会出错
3. PSpice -> Run 运行仿真
4. 程序自动调用 PSpice A/D 进行仿真, 并在文件夹下生成 .net 为后缀名的网络表
5. 在原理图中选择 PSpice -> Bias Points -> Enable 即可看到各节点的直流工作点数据

## 常见错误与解决方法

* ERROR Unable to open property mapping file: devparam.txt
  解决方法 : 重新设置路径 PSpice->Edit Simulation Profile-> Configuration Files->Library-> Library path->(orcad\tools\pspice\library