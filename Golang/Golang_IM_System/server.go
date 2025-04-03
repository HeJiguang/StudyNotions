package main

import (
	"fmt"
	"io"
	"net"
	"sync"
	"time"
)

// 服务器的类
type Server struct {
	Ip   string
	Port int

	// 在线用户的列表
	OnlineMap map[string]*User
	maplock   sync.RWMutex

	//广播消息的channel
	Message chan string
}

// 创建一个server初始化接口
func NewServer(ip string, port int) *Server {
	server := &Server{
		Ip:        ip,
		Port:      port,
		OnlineMap: make(map[string]*User),
		Message:   make(chan string),
	}

	return server
}

// 监听Message广播消息channel的goroutine，一但有消息发送给在线的User
func (this *Server) ListenMessage() {
	for {
		msg := <-this.Message

		// 将msg发送给在线的User
		this.maplock.Lock()
		for _, cli := range this.OnlineMap {
			cli.C <- msg
		}
		this.maplock.Unlock()
	}
}

// 广播消息的方法
func (this *Server) BroadCast(user *User, msg string) {
	sendMsg := "[" + user.Addr + "]" + user.Name + ":" + msg

	this.Message <- sendMsg
}

func (this *Server) Handler(conn net.Conn) {
	// 当前链接的业务
	fmt.Println("链接建立成功")

	user := NewUser(conn, this)

	user.Online()

	// 监听用户是否活跃的channel
	isLive := make(chan bool)

	// 接收顾客发送的消息
	go func() {
		buf := make([]byte, 4093)
		for {
			n, err := conn.Read(buf)
			if n == 0 {
				user.Offline()
				return
			}

			if err != nil && err != io.EOF {
				fmt.Println("Conn Read err :", err)
				return
			}

			// 提取用户消息
			msg := string(buf[:n-1])

			// 用户对msg进行消息处理
			user.DoMessage(msg)

			isLive <- true
		}
	}()

	// 阻塞当前函数
	for {
		select {
		case <-isLive:
			// 当前用户是活跃的，应该重置定时器

		case <-time.After(time.Second * 300):
			//已经超时
			//将当前user强制关闭

			user.SendMsg("你已强制下线")
			user.Offline()

			// 销毁用的资源
			close(user.C)

			//关闭链接
			conn.Close()

			// 退出当前的Handler
			return
		}
	}
}

// 启动服务器的方法
func (this *Server) Start() {
	//socket listen
	listener, err := net.Listen("tcp", fmt.Sprintf("%s:%d", this.Ip, this.Port))
	if err != nil {
		fmt.Println("net.Listen err", err)
		return
	}

	defer listener.Close()

	// 启动监听message的goroutine
	go this.ListenMessage()

	for {
		//accept
		conn, err := listener.Accept()
		if err != nil {
			fmt.Println("listener accept err: ", err)
			continue
		}

		//do handler
		go this.Handler(conn)
	}

	//close listen socket

}
