package warehouse;

import java.util.List;

public class MyMapa {

    private List<MyNode> nodeMap;




    private MyMapa(){

    }

    public MyMapa(List<MyNode> nodeMap){
        this.nodeMap = nodeMap;
    }


    public void setNodeMap(List<MyNode> nodeMap) {
        this.nodeMap = nodeMap;
    }

    public List<MyNode> getNodeMap() {
        return nodeMap;
    }

    @Override
    public String toString() {
        return "MyMapa{" +
                "nodeMap=" + nodeMap +
                '}';
    }
}
