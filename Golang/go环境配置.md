# 配置go开发

## 1. Linux下配置go开发环境

### 1.1 下载golong压缩包

我这里是使用xshell连接我的远程主机进行配置。首先要访问官网，如果没有上网小工具的话可以选择谷歌为中国准备的go镜像网站：https://go.p2hp.com/

进入网站，点击主页的下载按钮，找到Linux的安装包，下载到本地，再上传到服务器。

也可以在linux中直接下载，可以使用以下命令：

```bash
wget https://go.dev/dl/go1.21.5.linux-amd64.tar.gz
```

也可以选择国内镜像加速安装：

```bash
# 阿里云镜像（替换版本号）
wget https://mirrors.aliyun.com/golang/go1.21.5.linux-amd64.tar.gz  
```

### 1.2  解压安装go

下载完成之后，使用下面命令将go的压缩包进行解压：

```bash
sudo tar -zxvf go1.17.3.linux-amd64.tar.gz -C /usr/local/ 
```

解压完成后是将go安装在了/usr/local目录下面。

### 1.3 配置环境变量

接着需要将go配置在linux的环境变量中，使用下面命令打开shell环境变量配置文件：

```bash
vim ~/.bashrc
```

点击i进入编辑模式。

在其中最后添加如下代码：

```
# Go基础路径 
export GOROOT=/usr/local/go 
export GOPATH=$HOME/go 
export PATH=$PATH:$GOROOT/bin:$GOPATH/bin 
 
# 可选：国内开发加速 
export GOPROXY=https://goproxy.cn,direct  
export GOSUMDB=sum.golang.google.cn  
```

之后点击冒号，输入wq，保存退出。

输入以下命令重新加载配置文件：

```bash
source ~/.bashrc
```

### 1.4 环境配置验证：

使用下面命令可以检查自己是否安装成功：

```bash
go version  # 应显示类似 "go1.21.5 linux/amd64"
go env      # 检查GOROOT/GOPATH配置 
```

如果没有弹出说找不到go的路径，即是安装成功。

## 2. Windows下安装go开发环境

### 2.1 下载Windows安装包

访问Go官网下载页面或国内镜像站点，下载Windows的.msi安装包：

```bash
# 官方地址（需要网络工具）
https://go.dev/dl/

# 国内镜像地址
https://mirrors.aliyun.com/golang/go1.21.5.windows-amd64.msi
```

### 2.2 安装Go语言

1. 双击下载的.msi安装文件
2. 选择安装路径（默认C:\Go）
3. 保持所有选项默认，连续点击Next完成安装

### 2.3 配置环境变量

1. 右键【此电脑】→【属性】→【高级系统设置】→【环境变量】
2. 在系统变量中新建：
   - 变量名：GOROOT
   - 变量值：C:\Go  # 保持与安装路径一致
3. 编辑Path系统变量，添加：
   - %GOROOT%\bin
4. 新建用户变量：
   - 变量名：GOPATH
   - 变量值：%USERPROFILE%\go  # 项目代码存放路径

### 2.4 配置国内镜像（可选但推荐）

在PowerShell中运行：
```powershell
[System.Environment]::SetEnvironmentVariable('GOPROXY', 'https://goproxy.cn,direct', 'User')
[System.Environment]::SetEnvironmentVariable('GOSUMDB', 'sum.golang.google.cn', 'User')
```

### 2.5 验证安装

打开新的命令提示符或PowerShell：
```bash
go version  # 应显示类似 go1.21.5 windows/amd64
go env GOPATH  # 查看工作目录配置
go env GOPROXY # 查看镜像配置
```

## 3. 在VSCode中远程开发Go

### 3.1 安装必要插件
1. 在VSCode扩展商店搜索并安装：
   - Remote - SSH（微软官方远程开发插件）
   - Go (golang.go) 官方语言支持

### 3.2 配置SSH连接
1. 按Ctrl+Shift+P打开命令面板
2. 选择"Remote-SSH: Connect to Host..."
3. 输入服务器连接信息（格式：username@hostname）
4. 选择配置文件保存路径（默认~/.ssh/config）

### 3.3 设置远程工作区
```bash
# 在远程服务器创建项目目录
mkdir -p ~/go/src/hello-world
```

### 3.4 远程环境配置
1. 连接远程主机后打开终端
2. 安装必备工具：
```bash
# 安装gopls等开发工具
go install golang.org/x/tools/gopls@latest
```

### 3.5 调试配置
1. 创建launch.json调试配置文件：
```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Launch",
            "type": "go",
            "request": "launch",
            "mode": "auto",
            "program": "${workspaceFolder}"
        }
    ]
}
```

### 3.6 常见问题解决
- **工具安装失败**：
  ```bash
  # 设置模块模式
  go env -w GO111MODULE=on
  # 设置代理
  go env -w GOPROXY=https://goproxy.cn,direct
  ```
- **自动补全失效**：重新安装gopls并重启VSCode

