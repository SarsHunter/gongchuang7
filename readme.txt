op_type = 1: 取一层
op_type = 2: 取二层
op_type = 3: 放一层
op_type = 4: 放二层

color = 1: 红
color = 2: 绿
color = 3: 蓝

1234前后左右
5左前
6右前
7左后
8右后

0是zuo旋，1是you旋
====================================
{
  "type": "ArmTrack",
  "params": 
    { 
    "param": 1, 
    "op": 1 
    }
},

{
    "type": "CarTurn",
    "params": 
    {
      "angle": 90,
      "dir": 0
    }
},

{
    "type": "CarMove",
    "params": 
    {
      "dir": 1,
      "speed": 30.0,
      "dist": 90.0
    }
},

 { "type": "ColorSort", "params": {} },
  { "type": "ColorSort", "params": {} },
  { "type": "ColorSort", "params": {} },
=====================================（json旋转格式，最后一个不要加逗号，在[]内加入才有效）  
side =0;前半段
side =1;后半段


在 videothread.cpp 构造函数第 13~17 行：

redThreshold   ={170,180,43,255,46,255};   // 改这行调整红色
greenThreshold ={35,85,43,255,46,255};     // 改这行调整绿色
blueThreshold  ={100,130,43,255,46,255};   // 改这行调整蓝色

六个数字分别是：h_min, h_max, s_min, s_max, v_min, v_max