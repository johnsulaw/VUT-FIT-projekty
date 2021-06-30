/*
 * project: IJA 2020
 * authors: Jan Šulavík
 * content: Shelf class, describing a warehouse shelf
 * */

package warehouse;

import com.fasterxml.jackson.annotation.JsonIdentityInfo;
import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.ObjectIdGenerators;
import com.fasterxml.jackson.databind.annotation.JsonDeserialize;
import com.fasterxml.jackson.databind.util.StdConverter;
import javafx.event.EventHandler;
import javafx.scene.input.MouseButton;
import javafx.scene.input.MouseEvent;
import javafx.scene.paint.Color;
import javafx.scene.shape.Line;
import javafx.scene.shape.Shape;


//@JsonIdentityInfo(generator = ObjectIdGenerators.PropertyGenerator.class, property = "id")
public class Shelf {
    private String id;                         // shelf ID

    public GoodsType storedGoods;          // type of goods stored in this shelf
    public int quantity = 0;               // number of items in this shelf

    int currentStoredWeight = 0;    // counter of capacity of this storage space
    int maxCapacity;                // max capacity of this storage space
    private MyCoordinate position;

    /**
     * Default constructor for YML deserialization
     */
    public Shelf(){
    }

    /**
     * Shelf class object constructor
     * @param id Shelf's unique ID
     * @param maxCapacity Shelf's maximal capacity (kg)
     * @param storedGoods Type of goods stored in shelf
     */
    public Shelf(String id, int maxCapacity, GoodsType storedGoods, MyCoordinate position){
        this.id = id;
        this.maxCapacity = maxCapacity;
        this.storedGoods = storedGoods;
        this.position = position;
    }

    /**
     * Function sets position of Shelf
     * @param position  contains the coord_x and coord_y of shelf
     */
    public void setPosition(MyCoordinate position){this.position = position;}


    /**
     * Function returns the position of Shelf
     * @return positon which contains coord_x and coord_y of shelf
     */
    public MyCoordinate getPosition(){return this.position;}


    /**
     * Sets ID of shelf
     * @param id Shelf's ID
     */
    public void setId(String id) {
        this.id = id;
    }

    /**
     * Function returns shelf id
     * @return ID of shelf object
     */
    public String getId(){
        return this.id;
    }

    /**
     * Sets type of stored goods in shelf
     * @param storedGoods Type of goods stored in shelf
     */
    public void setStoredGoods(GoodsType storedGoods){
        this.storedGoods = storedGoods;
    }

    /**
     * Sets maximal capacity of shelf
     * @param maxCapacity Maximal capacity of shelf (kg)
     */
    public void setMaxCapacity(int maxCapacity){
        this.maxCapacity = maxCapacity;
    }

    /**
     * Sets quantity of items stored in this shelf
     * @param quantity Number of items stored in shelf
     */
    public void setQuantity(int quantity){
        put(this.storedGoods, quantity);
    }

    /**
     * Function puts a number of items of certain type
     * to shelf and adds to current stored weight of shelf.
     * If stored weight exceeds maximum capacity, item is not added.
     * @param goods Type of goods that are put in the shelf
     * @param quantity Number of items that are put into the shelf
     */
    public void put(GoodsType goods, int quantity){
        int added_weight = goods.getWeight() * quantity;

        if(added_weight + currentStoredWeight > maxCapacity){
            System.out.println("Shelf capacity exceeded!\n");
            return;
        }

        if(storedGoods != goods){
            System.out.println("Wrong type of goods added!\n");
            return;
        }

        // increment current stored weight of shelf
        // and quantity of items held
        this.quantity += quantity;
        this.currentStoredWeight += added_weight;

    }

    /**
     * Function returns number of stored items in shelf
     * @return Number of items stored in this shelf
     */
    public int size(){
        return this.quantity;
    }

    /**
     * Function removes one item of given type from shelf.
     * If shelf stores different kind of items from the ones to be
     * removed, nothing happens.
     * @param goods Type of item to be removed from shelf
     */
    public void remove(GoodsType goods){
        if(storedGoods != goods){
            System.out.println("Wrong type of goods to be removed!\n");
            return;
        }

        this.quantity -=1;
        this.currentStoredWeight -= goods.getWeight();
    }
}
