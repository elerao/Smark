# 简洁的 Markdown 编辑器 Smark

<p><img align="right" src="./README/logo.png" width="150"></p>

## 关于 Smark

说白了只是为了方便使用，没什么新奇的东西。我使用 [pandoc](http://johnmacfarlane.net/pandoc/) 来转化 `markdown`，但是我不想在修改文件时总是在编辑器、文字终端和浏览器间换来换去，因此我写了一个简单的编辑器，它在后台调用 `pandoc` 将当前编辑的 `markdown` 内容转化为 `HTML`，而后将 `HTML` 在 `smark` 中的浏览器中显示出来，就是这么回事。

在此之外，为方便用户使用，提高工作效率，在界面设计上 Smark 尽可能地凸显正在编辑的内容；隐藏所有的不需要的部件，包括菜单栏和工具栏；减少不必要的对话框弹出，将查找对话框 和 CSS 编辑器等都嵌在主窗口中，并根据用户的操作自动显示相应部件并切换焦点，而当您想隐藏这些部件时，万能的 `Esc` 键永远可以满足你的要求；按照不同的使用需求，Smark 可分为阅读、编辑和预览三种模式：

 + `阅读模式` ：Smark 默认以阅读模式打开 Markdown 文件，以 HTML 视图的方式显示 Markdown 文件内容；

 + `编辑模式` ：该模式主要用于编辑 Markdown 内容，只显示 Smark 的 Markdown 编辑器，而不显示 HTML 视图，配合全屏的话就相当于 `Gtihub` 的 `ZenMode`，可以作为一个 “专心致志写作软件”；

 + `预览模式` ：该模式并列显示 HTML 视图和 Markdown 编辑器，用于实时预览 Markdown 内容的 HTML 显示；

<p align="center">
    <image src="./README/screenshot.png" width="95%">
</p>

Smark 以阅读模式为基础模式，同样按 `Esc` 键可从任意模式回至阅读模式。我是一个键盘党，所以 Smark 具有一套完备的快捷键（详见[Smark快捷键列表](#hotkey)），仅仅使用键盘就能够搞定一切事情，这在使用上将带来巨大效率。此外 Smark 尽可能的简化用户的操作，只要不是必须由用户指明的，都由 Smark 自动进行：为此进行了许多细致的设计，例如根据窗口尺寸自动调整 HTML 视图和 Markdown 编辑器的布局（垂直布局和水平布局，包括 CSS 编辑器的布局）以达到最佳视觉效果；HTML 视图和 Markdown 编辑器的同步垂直滚动等等。正如您所见，由于个人的使用需求，Smark 引入 [MathJax](http://www.mathjax.org) 以拓展 Markdown 来显示数学公式（虽然 pandoc 也这么干了，但是那样很不方便），下面是一个简单的例子：

<p align="center">
    <image src="./README/example.png" width="80%">
</p>

Smark 依赖于 [`pandoc`](http://johnmacfarlane.net/pandoc/)、[`Qt 4.8`](http://qt-project.org/) 和 [MathJax](http://www.mathjax.org)，在此向上述软件包开发者们致敬。请注意继承于 pandoc 的发布协议，Smark 同样遵循 [LGPL](http://www.gnu.org/copyleft/gpl.html)，如有任何疑问请联系 [elerao.ao@gmail.com](elerao.ao@gmail.com)，我将尽快做出回复。

## 安装 Smark

 对于 Windows 平台，你可以直接从 [此处](http://pan.baidu.com/s/1pJnrX5x#dir/path=%2F%40%20Shared%2Fsmark) 直接下载当前版本 Smark 的 Win32 编译包。对于其它平台的用户，可以从 [此处](http://pan.baidu.com/s/1pJnrX5x#dir/path=%2F%40%20Shared%2Fsmark) 下载当前版本的 Smark 源代码在本平台上进行编译即可，一般地你可以按照如下步骤编译 Smark：

 1. 安装依赖的程序包:

        $ sudo apt-get install qtsdk
        $ sudo apt-get install pandoc

    如果你不使用 `apt-get` 作为软件包管理器，自己谷歌搜索如何安装这两个程序包

 2. 下载 [Smark 源代码](http://pan.baidu.com/s/1pJnrX5x#dir/path=%2F%40%20Shared%2Fsmark)，解压并编译:

        $ qmake -project
        $ qmake
        $ make

 3. 把编译出的可执行文件移动到你的 `[bin]` 路径下即可

## Smark 的快捷键列表

<p id="hotkey"></p>

 + 全局

        + Esc              : 隐藏 查找 / CSS 编辑 / Markdown 编辑部件 / 退出全屏显示
        + Tab              : 增加所选诸行的缩进（四个空格）
        + Ctrl + Tab       : 减小所选诸行的缩进（四个空格）

 + 文件菜单

        + Ctrl + N         : 新建 markdown 文件
        + Ctrl + O         : 打开 markdown 文件
        + Ctrl + S         : 保存当前 markdown 文件
        + Ctrl + Shift + S : 当前 markdown 文件另存为
        + Ctrl + W         : 关闭当前 markdown 文件
        + Ctrl + Q         : 退出 Smark 

 + 视图菜单

        + F6               : 预览模式视图
        + F7               : 阅读模式视图
        + F8               : 编辑模式视图
        + F11              : 进入 / 退出全屏显示
        + F10              : 显示 / 隐藏工具栏
        + F12              : 显示 / 隐藏菜单栏

 + 编辑菜单

        + F5               : 刷新 HTML 显示
        + Ctrl + C         : 复制
        + Ctrl + X         : 剪切
        + Ctrl + P         : 粘贴
        + Ctrl + F         : 查找
        + Ctrl + Z         : 撤消
        + Ctrl + Y         : 重做
        + Ctrl + G         : 编辑 CSS

 + 插入菜单

        + Ctrl + Shift + I : 插入图片
        + Ctrl + Shift + L : 插入链接
        + Ctrl + Shift + M : 插入数学公式

 + 格式菜单

        + Ctrl + B         : 加粗
        + Ctrl + I         : 倾斜
        + Ctrl + U         : 下划线
        + Ctrl + `         : 代码
        + Ctrl + '         : 引用
        + Ctrl + Down      : 下标
        + Ctrl + Up        : 上标
        + Ctrl + ]         : 加大字号
        + Ctrl + [         : 减小字号
        + Ctrl + L         : 内容左对齐
        + Ctrl + R         : 内容右对齐
        + Ctrl + E         : 内容居中

## TODO List

 + 当务之急是基于 `QScintilla` 文本编辑部件重新实现当前的 Markdown 编辑部件（包括 Markdown 语法高亮）；
 + 自带打印功能，目前只能先导出 HTML 再打印 HTML，直接把 `QWebView` 里面的内容打印出来的格式是乱的；
 + 支持使用本地 MathJax，目前是使用 MathJax CND 实现，但是 MathJax 的文件太多太碎，本地化的话发布未必方便；
 + 与 百度云、Google Drive、OneDrive 内容同步，这个以后再说。


 
