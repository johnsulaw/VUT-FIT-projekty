package warehouse;

import javafx.scene.paint.Color;

import java.util.List;

public class MyCart {

    private int id;
    private MyNode currentNode;
    private MyNode nextNode;
    private List<MyNode> pathNodes;
    private Basket content;
    private Request handlingRequest;
    //private Color color;
    private boolean parked;
    private boolean clicked;
    private boolean moving;


    public MyCart(){

    }

    public MyCart(int id, MyNode currentNode, MyNode nextNode, List<MyNode> pathNodes, Basket content, Request handlingRequest){
        this.id = id;
        this.currentNode = currentNode;
        this.nextNode = nextNode;
        this.pathNodes = pathNodes;
        this.content = content;
        this.handlingRequest = handlingRequest;
        //this.color = color;
        this.parked = true;
        this.clicked = false;
        this.moving = false;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getId() {
        return id;
    }

    public void setCurrentNode(MyNode currentNode) {
        this.currentNode = currentNode;
    }

    public MyNode getCurrentNode() {
        return currentNode;
    }

    public void setNextNode(MyNode nextNode) {
        this.nextNode = nextNode;
    }

    public MyNode getNextNode() {
        return nextNode;
    }

    public void setPathNodes(List<MyNode> pathNodes) {
        this.pathNodes = pathNodes;
    }

    public List<MyNode> getPathNodes() {
        return pathNodes;
    }

    /*public void setColor(Color color) {
        this.color = color;
    }

    public Color getColor() {
        return color;
    }*/

    public void setContent(Basket content) {
        this.content = content;
    }

    public Basket getContent() {
        return content;
    }

    public void setHandlingRequest(Request handlingRequest) {
        this.handlingRequest = handlingRequest;
    }

    public Request getHandlingRequest() {
        return handlingRequest;
    }

    public void setClicked(boolean clicked) {
        this.clicked = clicked;
    }

    public boolean isClicked() {
        return clicked;
    }

    public void setParked(boolean parked) {
        this.parked = parked;
    }

    public boolean isParked() {
        return parked;
    }

    public void setMoving(boolean moving) {
        this.moving = moving;
    }

    public boolean isMoving() {
        return moving;
    }

    @Override
    public String toString() {
        return "MyCart{" +
                "id=" + id +
                ", currentNode=" + currentNode +
                ", nextNode=" + nextNode +
                ", pathNodes=" + pathNodes +
                ", content=" + content +
                ", color=" + //color +
                ", parked=" + parked +
                ", clicked=" + clicked +
                ", moving=" + moving +
                '}';
    }
}
