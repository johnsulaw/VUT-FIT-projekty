package warehouse;

import java.util.ArrayList;

public class Request {
    int id;

    public ArrayList<GoodsType> goods = new ArrayList<>();
    public ArrayList<Integer> quantity = new ArrayList<>();

    /* variable holding the actual basket, that will
       get the goods from shelves */
    Basket basket;

    /**
     * Default constructor for YML deserialization
     */
    public Request(){
    }

    /**
     * Function sets the list with types of goods to be picked up.
     * @param goods List of goods types requested.
     */
    public void setGoods(ArrayList<GoodsType> goods){
        this.goods = goods;
    }

    /**
     * Function sets the list with quantities of required items.
     * @param quantity List of integers representing required number of items.
     */
    public void setQuantity(ArrayList<Integer> quantity){
        this.quantity = quantity;
    }

    /**
     * Function sets request's ID.
     * @param id ID of request.
     */
    public void setId(int id){
        this.id = id;
    }

    public int getId(){return this.id;}

    /*public ArrayList<GoodsType> giveName(){return this.goods;}

    public ArrayList<Integer> getQuantity() { return this.quantity;}*/

    /**
     * Function checks if request is placed correctly.
     */
    public void checkRequest(){
        if (quantity.size() != goods.size()){
            System.out.println("Bad request.\n");
        }
    }
}
