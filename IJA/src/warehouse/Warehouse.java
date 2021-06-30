/*
 * project: IJA 2020
 * authors: Jan Šulavík
 * content: Warehouse class definition
 *          this class holds data about the whole warehouse
 * */

package warehouse;

import warehouse.*;
import java.util.ArrayList;

public class Warehouse {
     public ArrayList<Shelf> shelves;       // List of shelves in the warehouse
     public ArrayList<Basket> baskets;      // List of baskets in the warehouse
     public ArrayList<Request> requests;    // List of requests that need to be handled
     public ArrayList<Shelf> shelvesOnLine;
     public ArrayList<MyNode> mapa;
     public ArrayList<MyNode> nodes;


     /**
     * Function initializes warehouse data structures
     */
    public Warehouse(){
         shelves = new ArrayList<>();
         baskets = new ArrayList<>();
         requests = new ArrayList<>();
         shelvesOnLine = new ArrayList<>();
         mapa = new ArrayList<>();
         nodes = new ArrayList<>();
         //create 10 default baskets
        for(int i = 0; i < 10; i++){
            this.addBasket(i);
            this.baskets.get(i).setParked(true);

        }
    }
    /**
     * Function initializes all shelves in the warehouse
     * @param shelves List of shelves
     */
    public void setShelves(ArrayList<Shelf> shelves){
        this.shelves = shelves;
    }

    /**
     * Function returns all shelves in the warehouse
     * @return List of shelves in the warehouse
     */
    public ArrayList<Shelf> getShelves(){
        return this.shelves;
    }

    /**
     * Function initializes all baskets in the warehouse
     * @param baskets List of baskets
     */
    public void setBaskets(ArrayList<Basket> baskets){
        this.baskets = baskets;
    }

    public ArrayList<Basket> getBaskets() {
        return baskets;
    }

    /**
     * Function initializes all requests that will be processed
     * @param requests List of requests
     */
    public void setRequests(ArrayList<Request> requests){
        this.requests = requests;
    }

    public ArrayList<Request> getRequests(){
        return this.requests;
    }

    public void setShelvesOnLine(ArrayList<Shelf> shelvesOnLine) {
        this.shelvesOnLine = shelvesOnLine;
    }

    public void setMapa(ArrayList<MyNode> mapa) {
        this.mapa = mapa;
    }

    public ArrayList<MyNode> getMapa() {
        return mapa;
    }

    public void setNodes(ArrayList<MyNode> nodes) {
        this.nodes = nodes;
    }

    public ArrayList<MyNode> getNodes() {
        return nodes;
    }

    /**
     * Function returns index of node with ID specified in the string
     * @param index
     * @return
     */
    public int NodeIndex(String id){
        int pos = 180;
        for(MyNode node: this.nodes){
            if(node.getId().equals(id) == true){
                pos = this.nodes.indexOf(node);
                return pos;
            }
        }
        return pos;
    }

    /**
     * Adds one basket to basket list of warehouse
     */
    public void addBasket(int id){
        for( Basket basket : baskets){
            if(basket.getId() == id){
                System.out.println("Basket with this ID already exists.\n");
                return;
            }
        }
        Basket basket = new Basket(id);

	// TODO nastavit vsem kosikum pri vytvoreni vychozi node
        //basket.setCurrentNode(this.nodes.get());

        if(baskets != null){
            baskets.add(basket);
        }
    }

    /**
     * Function searches the warehouse for a shelf with
     * required goods type and at least one item of given type.
     * @param goods Type of goods searched.
     * @return Shelf object with required goods type, if no such shelf exists
     *         return null.
     */
    public Shelf findGoods(GoodsType goods){
        for( Shelf shelf : this.shelves ){
            if((shelf.storedGoods.name.equals(goods.name)) && (shelf.quantity > 0)){
                return shelf;
            }
        }
        return null;
    }

    public void completeRequests(){
        for(Request request : this.requests){
            for(GoodsType goods : request.goods){
                Shelf shelf = findGoods(goods);
                goods.weight = shelf.storedGoods.weight;
            }
        }
    }


    /**
     * Function checks if request can fit into one basket, if it cannot,
     * the request is split.
     */
    public void checkRequestsSize(){
        int basket_capacity = 500;
        ArrayList<Request> new_requests = new ArrayList<>();

        for(Request request : this.requests){
            int total_weight = 0;
            int index = 0;
            boolean too_heavy = false;

            for(GoodsType goods : request.goods){
                total_weight += goods.weight * request.quantity.get(index);

                if(total_weight > basket_capacity){
                    too_heavy = true;
                    break;
                }
                index++;
            }
            if(too_heavy){
                Request new_request = new Request();

                ArrayList<GoodsType> new_goods = new ArrayList<>();
                ArrayList<Integer> new_quantity = new ArrayList<>();

                int k = 0;
                while(k < index){
                    new_goods.add(request.goods.get(k));
                    new_quantity.add(request.quantity.get(k));

                    request.goods.remove(k);
                    request.quantity.remove(k);

                    new_request.goods = new_goods;
                    new_request.quantity = new_quantity;

                    new_request.setId(this.requests.size() + 1);
                    new_requests.add(new_request);

                    k++;
                }
            }
        }

        ArrayList<Request> adding_new_quantity_requests = new ArrayList<>();

        /* check if quantity of each item can be carried by the basket
           if not, split the request further */
        for(int y = 0 ; y < this.requests.size(); y++) {
            ArrayList<Request> new_quantity_requests = this.checkQuantities(this.requests.get(y));
            if(!new_quantity_requests.isEmpty()){
                // if new requests were made by checkQuantities function, remove the original one
                this.requests.remove(y);
                // add newly created requests to an array
                for(Request new_quantity_request : new_quantity_requests){
                    adding_new_quantity_requests.add(new_quantity_request);
                }
            }
        }

        // if new requests were made by checkQuantities function, add them to warehouse requests
        if(!adding_new_quantity_requests.isEmpty()){
            for(Request request : adding_new_quantity_requests){
                this.requests.add(request);
            }
        }

        // add new split requests to warehouse
        for(Request request : new_requests){
            this.requests.add(request);
            this.checkRequestsSize(); // check again in case the request is still too big even after splitting

        }

        // resetting requests ID's after splitting and making new ones
        for(int id=0; id < this.requests.size(); id++){
            this.requests.get(id).setId(id);
        }
    }

    /**
     * Function checks if the quantity of an item in the request can be carried by the basket,
     * if the quantity wont fit into the basket, then split the request into multiple requests so
     * it fits.
     * @param request Request in which the quantities are checked.
     * @return Array of new requests made by splitting, if no splitting was needed, function
     * returns an empty array.
     */
    public ArrayList<Request> checkQuantities(Request request){
        int basket_capacity = 500;

        ArrayList<Request> new_requests = new ArrayList<>();

        for(int i = 0 ; i < request.goods.size(); i++){
            int goods_quantity = request.quantity.get(i);
            int total_weight = goods_quantity * request.goods.get(i).getWeight();

            if(total_weight > basket_capacity){
                // maximal quantity of items of given type, that the basket can carry at one moment
                int max_quantity = basket_capacity / request.goods.get(i).getWeight();
                // quantity of items, that the basket needs to carry
                int required_quantity = request.quantity.get(i);
                // number of requests that need to be created in order to fit the items into baskets
                int num_of_needed_requests = required_quantity / max_quantity;
                num_of_needed_requests += 1; // round the number of new needed requests up 1

                // create as many new requests as needed
                for(int x = 0; x < num_of_needed_requests; x++){
                    Request new_request = new Request();

                    ArrayList<GoodsType> new_goods = new ArrayList<>();
                    ArrayList<Integer> new_quantity = new ArrayList<>();

                    new_goods.add(request.goods.get(i));

                    /* if its the last iteration, take the remaining quantity of items
                    *  else use computed maximal quantity of items that will fit into the basket */
                    if(x + 1 == num_of_needed_requests){
                        new_quantity.add(i, required_quantity - (x*max_quantity));
                    }else{
                        new_quantity.add(i, max_quantity);
                    }

                    new_request.goods = new_goods;
                    new_request.quantity = new_quantity;
                    new_request.setId(this.requests.size() + 1 + x);

                    new_requests.add(new_request);
                }
            }
        }
        return new_requests;
    }

    /**
     * Function assigns unhandled requests to stationary baskets in the warehouse.
     * The assigned request is deleted from warehouse requests and added
     * to basket.
     */
    public void loadRequestToBasket(){
        for(Basket basket : this.baskets){
            if(!basket.isMoving()){
                if(!this.requests.isEmpty()){
                    Request request = this.getRequests().remove(0);
                    basket.bindRequest(request);
                    basket.setMoving(true);
                }
            }
        }
    }
}
