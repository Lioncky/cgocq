# CGoCQ
对gocqhttp进行了全面的C++封装 并提供了易++的接口   
特供Windows Server 不支持其他操作系统  
*注意 此项目目前仍于开发中 不可直接使用*  

## 为什么基于go
go提供了良好的网络库 gocq虽然基于mirai但为原生协议实现 效率不输于mirai  
另外这个项目已经存在好几年了 也还是比较稳定的  

# 使用 Use
GoCQ配置 [gocqhttp](https://github.com/Mrs4s/go-cqhttp#readme) **快照基于2023/08/30**
![image](https://github.com/Lioncky/cgocq/assets/73447685/8fe9c342-4e88-46e1-a768-ec3645583428)
> 1. 保证GoCQ能够正常运行  
> 2. 配置WS反向端口默认为8080  
> 3. 通过修改配置此项GoCQ的路径  
> 4. 双击🚀发射
> 

## 反向 WndCilent
> 通过挂载Win32原生的窗口事件实现异步跨进程数据通讯  
> 当前仅支持OneBot完全模式 日后可能划分到具体事件代码  
> 通过修改cgo.ini来配置此项  
> 

# 未来 Future
- [x] 基于std::map实现的原生AsyncWebsocket接口
- [x] 基于Win32的原生跨进程通讯服务
- [ ] 对Lua提供直接或间接性的支持 C<-->Lua
- [ ] 对易++提供更多细节的支持
- [ ] 更详细的事件划分 
- [ ] 更具体的细节实现
