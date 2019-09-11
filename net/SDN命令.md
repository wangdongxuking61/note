SDN命令
```
- [x] mininet
sudo mn --topo=single,3 --mac --switch ovsk --controller=remote
sudo mn --topo=tree,depth=2,fanout=1 --controller=remote --switch ovsk,protocols=OpenFlow13
- [x] MiniNAM
sudo python mininet/MiniNAM.py --topo=tree,depth=2,fanout=1  --controller=remote
sudo python mininet/MiniNAM.py --custom mininet/create_topo.py --topo mytopo,3  --controller=remote

- [x] ryu
sudo ryu-manager --verbose ryu.app.simple_switch_13
sudo ryu-manager sp.py --observe-links	# —observe-links 自动下发LLDP，用于拓扑发现。

- [x] pycharm
sudo ~/pycharm-2017.2.4/bin/pycharm.sh &
- [x] ovs
交换机看流项 sudo ovs-ofctl -O openflow13 dump-flows s1

- [x] floodlight
java -jar target/floodlight.jar
http://localhost:8080/ui/index.html

sudo mn --custom fattree.py --topo mytopo --controller=remote --switch ovsk,protocols=OpenFlow10
```