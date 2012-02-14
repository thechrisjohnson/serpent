public class Block {

    private long upper;
    private long lower;

    public Block(long lower, long upper) {
        this.lower = lower;
        this.upper = upper;
    }

    public long getLower() {
        return lower;
    }

    public void setLower(long lower) {
        this.lower = lower;
    }

    public long getUpper() {
        return upper;
    }

    public void setUpper(long upper) {
        this.upper = upper;
    }

}
