# Git
Git是一个开源的分布式版本控制系统，用于敏捷高效地处理任何或小或大的项目。Git是用C语言写成的，它的速度飞快，占用内存也很少。Git支持全面丰富的分支管理，便于多人协作。
## Git的使用
- 注册git管理账户
~~~
git config --global user.name "your name"
git config --global user.email "your email"
~~~
- 创建本地仓库
~~~
mkdir myproject /* 创建一个目录 */
cd myproject    /*  进入目录    */
git init        /*  初始化仓库  */
~~~
- 添加文件到本地仓库
~~~
git add filename /* 添加文件到暂存区 */
git add .      /* 添加所有文件到暂存区 */
git commit -m "commit message"  /* 提交到本地仓库 */
~~~
- 查询状态
~~~
git status      /* 查看当前仓库的状态 */
~~~
- 查看提交历史
~~~
git log         /* 查看提交历史 */
~~~
- 回溯版本
~~~
git reset --hard commit_id  /* 回溯到指定版本 */
git reset --hard HEAD^  /* 回退到上一个版本 */
~~~
- 克隆远程仓库
~~~
git clone https://github.com/username/repository.git  /* 克隆远程仓库 */
~~~
- 推送本地仓库到远程仓库
~~~
git push origin master  /* 推送本地仓库到远程仓库 */
~~~
- 从远程仓库拉取最新代码
~~~
git pull origin master  /* 从远程仓库拉取最新代码 */
~~~
## KEIL下Git的使用技巧
- 提交时在状态栏上的Projet选项卡选择Clean Targets，可以清除编译产生的中间文件，减少提交大小。
  [![pE1SxYQ.png](https://s21.ax1x.com/2025/02/24/pE1SxYQ.png)](https://imgse.com/i/pE1SxYQ)


