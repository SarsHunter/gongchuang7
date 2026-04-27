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

0是右旋，1是左旋
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
=====================================（json旋转格式，最后一个不要加逗号，在[]内加入才有效）  