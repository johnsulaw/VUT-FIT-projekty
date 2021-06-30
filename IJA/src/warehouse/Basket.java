package warehouse;

import java.util.*;

import javafx.application.Platform;
import javafx.scene.shape.Circle;

public class Basket {
    private int id;

    private MyNode currentNode;
    private MyNode nextNode;
    private List<MyNode> pathNodes = new ArrayList<>();
    private List<MyNode> goalNodes = new ArrayList<>();
    private Circle visibleBasket;

    Map<GoodsType, Integer> contents = new HashMap<>(); // contents of this basket

    int currentStoredWeight = 0;               // counter of capacity of this storage space
    int maxCapacity = 500;                     // max capacity of basket

    private Request handlingRequest;                        // request for this basket

    private boolean parked;
    private boolean clicked;
    private boolean moving;


    public Basket(int id){
        this.id = id;
    }

    public Basket(int id, MyNode currentNode, MyNode nextNode, List<MyNode> pathNodes, Request handlingRequest, List<MyNode> goalNodes) {
        this.id = id;
        this.currentNode = currentNode;
        this.nextNode = nextNode;
        this.pathNodes = pathNodes;
        this.handlingRequest = handlingRequest;
        this.goalNodes = goalNodes;
        this.parked = true;
        this.clicked = false;
        this.moving = false;

    }

    /**
     * Function sets basket's id.
     * @param id ID of basket
     */
    public void setId(int id){ this.id = id; }

    /**
     * Function returns basket's id.
     * @return ID of basket.
     */
    public int getId(){
        return this.id;
    }

    public void setCurrentNode(MyNode currentNode){ this.currentNode = currentNode; }

    public MyNode getCurrentNode(){
        return this.currentNode;
    }

    public void setNextNode(MyNode nextNode){
        this.nextNode = nextNode;
    }

    public MyNode getNextNode(){
        return this.nextNode;
    }

    public void setHandlingRequest(Request handlingRequest){
        this.handlingRequest = handlingRequest;
    }

    public Request getHandlingRequest(){
        return this.handlingRequest;
    }

    public void setClicked(boolean clicked){
        this.clicked = clicked;
    }

    public boolean isClicked(){
        return this.clicked;
    }

    public void setParked(boolean parked){
        this.parked = parked;
    }

    public boolean isParked(){
        return this.parked;
    }

    public void setMoving(boolean moving){
        this.moving = moving;
    }

    public boolean isMoving(){
        return this.moving;
    }

    public List<MyNode> getPathNodes() {
        return pathNodes;
    }

    public void setPathNodes(List<MyNode> pathNodes) {
        this.pathNodes = pathNodes;
    }

    public List<MyNode> getGoalNodes() {
        return this.goalNodes;
    }

    public void setGoalNodes(List<MyNode> goalNodes) {
        this.goalNodes = goalNodes;
    }

    /**
     * Function puts item into basket
     *
     * @param goods Goods type that is being put to basket
     */
    public void put(GoodsType goods) {
        Map<GoodsType, Integer> content = this.contents;

        if (goods.getWeight() + currentStoredWeight > maxCapacity) {
            System.out.println("Basket capacity exceeded!\n");
            return;
        }

        // check if goods type is already in basket, if not add to hashmap
        if (content.containsKey(goods)) {
            this.contents.put(goods, this.contents.get(goods) + 1);
        } else {
            this.contents.put(goods, 1);
        }

        this.currentStoredWeight += goods.getWeight();

    }

    /**
     * Function retrieves specified number of items of given type from a shelf and adds it to basket.
     * @param shelf Shelf from which the item is taken
     * @param goods Item type that is put to basket
     * @param quantity Number of items to retrieve
     */
    public void getItem(Shelf shelf, GoodsType goods, int quantity) {
        // check if shelf exists
        if (shelf == null) {
            return;
        }

        // check if shelf contains right type of goods
        if (shelf.storedGoods != goods) {
            return;
        }

        // check if shelf contains at least one wanted item
        if (shelf.quantity <= 0) {
            return;
        }

        if (shelf.quantity <= quantity) {
            System.out.println("There is not enough items in the shelf");
            return;
        }

        // add items to basket and remove it from the shelf
        for (int x = quantity; x > 0; quantity = x--) {
            this.put(goods);
            shelf.remove(goods);
        }
    }

    /**
     * Function finds path made of nodes from basket's current position
     * to position specified by parameter.
     * @param goalNode Node to which is the function finding a path.
     * @return List of neighbouring nodes that define the path.
     */
    public ArrayList<MyNode> findPath(MyNode goalNode, Warehouse warehouse){
          ArrayList<MyNode> openList = new ArrayList<>();     // holds nodes to be visited
          ArrayList<MyNode> closedList = new ArrayList<>();   // holds already visited nodes

          openList.add(this.currentNode);

          while(!openList.isEmpty()){
              MyNode node = (MyNode)openList.remove(0);
              // found goal node
              if(node == goalNode){
                  ArrayList<MyNode> path = new ArrayList<>();
                  while(node.pathParent != null){
                      path.add(0, node);
                      node = node.pathParent;
                  }
                  for(MyNode node1 : warehouse.getNodes()){
                      node1.setPathParent(null);
                  }
                  return path;
              }else{
                  closedList.add(node);

                  // adding neighbours of node to open list
                  List<MyNode> neighboursList = node.getNextNodes();
                  //System.out.println("toto su neighbours" + node.getNextNodes());
                  for (MyNode Node : neighboursList){
                      if(!closedList.contains(Node) && !openList.contains(Node)){
                          Node.pathParent = node;
                          openList.add(Node);
                      }
                  }
              }
          }



        // no path was found
        return null;
    }

    /**
     * Function links a request with basket.
     * @param request Request that is being assigned to basket.
     */
    public void bindRequest(Request request) {
        int total_weight = 0;
        boolean too_heavy = false;

        /* count total weight of order to check
         * if order fits into one basket */
        for (int i = 0; i < request.goods.size(); i++) {
            GoodsType item = request.goods.get(i);
            int item_quantity = request.quantity.get(i);

            int weight = item.weight * item_quantity;
            total_weight += weight;
            if (total_weight > this.maxCapacity) {
                too_heavy = true;
                break;
            }
        }

        // if request cannot be handled by one basket
        // this should not occur, as the requests are checked before binding
        if (too_heavy) {
            System.out.println("Request is too big and wont fit into one basket!");
            return;
        }

        this.handlingRequest = request;
        this.moving = true;
    }

     /**
     * Function looks at goods in request assigned to basket and tries
     * to find a shelf with that goods type in the warehouse. If a shelf is
     * found, function finds node corresponding to the shelf and that
     * node is added to list of nodes, that the basket needs to visit
     * in order to get required goods.
     * @param warehouse Warehouse in which the function should try to
     *                  find required goods.
     */
    public void findGoalNodes(Warehouse warehouse){
            if(this.handlingRequest != null){
                for(GoodsType goods : this.handlingRequest.goods){
                    Shelf shelf = warehouse.findGoods(goods);
                    if (shelf == null){
                        System.out.println("Cannot find goods of type: " + goods.getName());
                    }
                    for(MyNode node : warehouse.nodes){
                        if(node.getId().equals(shelf.getId())){
                            this.goalNodes.add(node);
                        }
                    }
                }
            }


    }



    /**
     * Function empties the basket.
     */
    public void empty() {
        this.handlingRequest = null;
        this.moving = false;
        this.currentStoredWeight = 0;
        this.contents.clear();
    }



    @Override
    public String toString(){
        return "MyCart{" +
                "id=" + id +
                ", currentNode=" + currentNode +
                ", nextNode=" + nextNode +
                ", pathNodes=" + pathNodes +
                ", parked=" + parked +
                ", clicked=" + clicked +
                ", moving=" + moving +
                '}';
    }
}
