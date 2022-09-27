# 完成了一个简易版的shell
实现了linux上现成的基础命令(单一的), 例如, ls, cat, echo, 等等
实现了管道, 例如cat hello.txt | sort| uniq, 这种
实现了重定向, 分别实现了输出重定向和输入重定向, 例如echo 123 > a.txt, cat hello.txt | sort > b.txt, cat < hello.txt 等等
实现了后台执行&