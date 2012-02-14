import java.util.List;

public class Key {

    private long first, second, third, fourth;
    private List<Block> subkeys = null;

    public Key(Block key) {
        first = key.getLower();
        second = key.getUpper();
        third = Constants.mask;
        fourth = 0x0L;
        generateSubkeys();
    }

    public Key() {}

    public void generateSubkeys() {
        int[] w = new int[132];
        //expand to 33 128bit subkeys K0 - K32
        //write the key (prekey) as (8 32 bit words) w-8 to w-1
        //using the S-boxes - starting at S3 - we convert wi into ki
        //subkey Ki = k4i, k4i+1, k4i+2, and k4i+3
        //permute using initPerm to get K-hat i
    }

    public Block getSubkey(int subkey) {
        if (subkeys == null || subkey >= subkeys.size()) {
            return null;
        }
        return subkeys.get(subkey);
    }


}
