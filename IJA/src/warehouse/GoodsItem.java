package warehouse;

public class GoodsItem {
    GoodsType type;

    public GoodsItem(GoodsType type){
        this.type = type;
    }

    public GoodsType getType(){
        return this.type;
    }

    public int getWeight(){
        return this.getType().weight;
    }
}
