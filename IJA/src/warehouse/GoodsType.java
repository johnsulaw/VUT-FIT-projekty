package warehouse;

import java.util.ArrayList;

public class GoodsType {
    public String name;    // name of this type of goods
    public int weight;     // weight of this type of goods

    /**
     * Default constructor for YML deserialization.
     */
    public GoodsType(){

    }

    /**
     * Constructor for a new goods type.
     * @param name Name of the goods type.
     * @param weight Weight of the goods type.
     */
    public GoodsType(String name, int weight){
        this.name = name;
        this.weight = weight;

    }

    /**
     * Function sets name of goods type.
     * @param name Name of goods type.
     */
    public void setName(String name){
        this.name = name;
    }

    /**
     * Function sets weight of goods type.
     * @param weight Weight of goods type.
     */
    public void setWeight(int weight){
        this.weight = weight;
    }


    /**
     * Function returns name of goods type.
     * @return Name of this goods type.
     */
    public String getName(){
        return this.name;
    }

    /**
     * Function returns weight of the goods type.
     * @return Weight of this goods type.
     */
    public int getWeight() { return this.weight; }


    @Override
    public String toString(){
        return name;
    }
}