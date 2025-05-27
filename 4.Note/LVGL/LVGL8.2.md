# LVGL8.2版本的使用
## 目录

## LVGL基础
### 屏幕的创建与切换
~~~c
/* 1.创建屏幕 */
lv_obj_t * MainScreen = lv_obj_create(NULL);

/* 2.创建完屏幕需要加载才可以显示屏幕的内容 */
/* 2.1 加载屏幕，无动画效果 */
lv_scr_load(lv_obj_t * scr);

/* 2.2 加载屏幕，有动画效果 */
lv_scr_load_anim(lv_obj_t * new_scr, lv_scr_load_anim_t anim_type, uint32_t time, uint32_t delay, bool auto_del);
typedef enum {
    LV_SCR_LOAD_ANIM_NONE,          /* 无动画效果   */
    LV_SCR_LOAD_ANIM_OVER_LEFT,     /* 从左到右覆盖 */
    LV_SCR_LOAD_ANIM_OVER_RIGHT,    /* 从右到左覆盖 */
    LV_SCR_LOAD_ANIM_OVER_TOP,      /* 从上到下覆盖 */
    LV_SCR_LOAD_ANIM_OVER_BOTTOM,   /* 从下到上覆盖 */
    LV_SCR_LOAD_ANIM_MOVE_LEFT,     /* 从左到右移动 */
    LV_SCR_LOAD_ANIM_MOVE_RIGHT,    /* 从右到左移动 */
    LV_SCR_LOAD_ANIM_MOVE_TOP,      /* 从上到下移动 */
    LV_SCR_LOAD_ANIM_MOVE_BOTTOM,   /* 从下到上移动 */
    LV_SCR_LOAD_ANIM_FADE_ON,       /* 渐隐效果     */
} lv_scr_load_anim_t;

/* 3.获取当前的活动屏幕 */
lv_obj_t * CurrentScreen = lv_scr_act(void);
~~~
### 图层
- 图层的顺序（系统层为最顶层）

    lv_layer_sys()      系统层

    lv_layer_top()      顶层

    lv_layer_sys_bg()   活动层
~~~c
/* 1.将一个屏幕放置在顶岑 */
lv_obj_t * TopScreen = lv_obj_create(lv_layer_top());

/* 2.将一个部件放在顶部显示 */
lv_obj_move_foreground(lv_obj_t * obj);

/* 3.将一个部件放在底部显示 */
lv_obj_move_background(lv_obj_t * obj);
~~~
### 对象的删除
~~~c
/* 1.立即删除对象本身跟其子对象 */
void lv_obj_del(lv_obj_t * obj);

/* 2.在下一个周期删除对象本身跟其子对象 */
void lv_obj_del_async(lv_obj_t * obj);

/* 3.删除对象的所有子对象 */
void lv_obj_clean(lv_obj_t * obj);
~~~
### Parts（部件的组成部分）
| **部件名称**           | **描述**                                                      |
|----------------------|-------------------------------------------------------------|
| `LV_PART_MAIN`        | 类似矩形的背景                                                |
| `LV_PART_SCROLLBAR`   | 滚动条                                                        |
| `LV_PART_INDICATOR`   | 指标，例如用于滑块、条、开关或复选框的勾选框                    |
| `LV_PART_KNOB`        | 像手柄一样可以抓取调整值                                        |
| `LV_PART_SELECTED`    | 表示当前选择的选项或部分                                        |
| `LV_PART_ITEMS`       | 如果小部件有多个相似的元素（例如表格单元格）                    |
| `LV_PART_TICKS`       | 刻度上的刻度，例如对于图表或仪表                                |
| `LV_PART_CURSOR`      | 用于标记一个特定的地方，例如文本区域或图表的光标                |
| `LV_PART_CUSTOM_FIRST`| 可以从这里添加自定义部件                                        |
### States（状态）
| **常用状态名称**           | **描述**                                                      |
|----------------------|----------|
| `LV_STATE_DEFAULT`    | 默认状态，没有其他状态                                         |
| `LV_STATE_FOCUSED`    | 聚焦状态，例如用于按钮、文本框、开关、复选框等                    |
| `LV_STATE_PRESSED`    | 按下状态，例如用于按钮、开关、复选框等                            |
| `LV_STATE_CHECKED`    | 选中状态，例如用于复选框                                        |
| `LV_STATE_DISABLED`   | 禁用状态，例如用于按钮、文本框、开关、复选框等                    |

~~~c
/* 1.设置对象的状态（只保留设置的状态） */
void lv_obj_set_state(lv_obj_t * obj, lv_state_t state);

/* 2.添加对象的状态（添加状态） */
void lv_obj_add_state(lv_obj_t * obj, lv_state_t state);

/* 3.清除对象的状态 */
void lv_obj_clear_state(lv_obj_t * obj, lv_state_t state);

/* 4.获取对象的状态 */
lv_state_t lv_obj_get_state(const lv_obj_t * obj);

/* 5.判断对象是否有某种状态 */
bool lv_obj_has_state(const lv_obj_t * obj, lv_state_t state);
~~~
### Styles（样式）
#### 样式的创建与设置
~~~c
/* 1.设置对象的样式有两种方法 */
/* 1.1 直接设置样式 例:设置背景颜色 */
lv_obj_set_style_bg_color(lv_obj_t * obj, lv_color_t color, lv_state_t state);

/* 1.2 通过样式设置 例:设置背景颜色 */
lv_style_t * style = lv_style_create();
lv_style_set_bg_color(style, LV_STATE_DEFAULT, lv_color_hex(0xFFFFFF));
lv_obj_add_style(obj, style, LV_PART_MAIN);

/* 2.删除样式 例:删除滚动条样式 */
lv_obj_remove_style(obj, NULL, LV_PART_SCROLLBAR);  /* 删除所有滚动条样式   */
lv_obj_remove_style(obj, style, LV_PART_SCROLLBAR); /* 删除指定的滚动条样式 */
~~~

#### 样式属性
[详情点此查看](https://book.yangben365.com/view/dto)
### Flags（宏开关）
| **常用宏名称**           | **描述**                                                      |
|----------------------|----------|
| `LV_FLAG_HIDDEN`      | 隐藏标志，用于隐藏对象                                        |
| `LV_FLAG_CLICK_FOCUS` | 点击聚焦标志，用于使对象被点击时自动获取焦点                    |
| `LV_FLAG_CHECKABLE`   | 对象被点击时切换选中状态                                |            |
| `LV_FLAG_SCROLLABLE`  | 可滚动标志，用于表示对象是否可以滚动                            |
~~~c
/* 1.设置对象标志 */
void lv_obj_add_flag(lv_obj_t * obj, lv_obj_flag_t f);

/* 2.清除对象标志 */
void lv_obj_clear_flag(lv_obj_t * obj, lv_obj_flag_t f);

/* 3.判断对象是否有某种标志 */
bool lv_obj_has_flag(const lv_obj_t * obj, lv_obj_flag_t f)
~~~





