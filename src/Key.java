import java.util.List;
import java.util.LinkedList;

public class Key {

    private static final int phi = 0x9e3779b9;

    private long first, second, third, fourth;
    private List<Block> subkeys = null;

    public Key(Block key) {
        first = key.getLower();
        second = key.getUpper();
        third = 0x8000000000000000L;
        fourth = 0x0L;
        generateSubkeys();
    }

    public Key() {}

    public void generateSubkeys() {
        int[] w = new int[140];
        int[] k = new int[132];
        int sbox = 3;
        
        // Move pre-key into array
        w[0] = (int) (first >> 32);
        w[1] = (int) first;
        w[2] = (int) (second >> 32);
        w[3] = (int) second;
        w[4] = (int) (third >> 32);
        w[5] = (int) third;
        w[6] = (int) (fourth >> 32);
        w[7] = (int) fourth;

        //Generate 132 more words
        for (int i = 0; i < 132; i++) {
            w[i+8] = (w[i-8] ^ w[i-5] ^ w[i-3] ^ w[i-1] ^ phi ^ i) << 11;
        }

        //Pass words through S-boxes
        for (int i = 0; i < 132; i++) {

            k[i] = SBox.sBox(w[i], sbox);
            
            //Switch s-boxes if needd
            if (i != 0 && i % 4 == 0) {
                sbox--;
                if (sbox < 0) {
                    sbox = 7;
                }
            }
        }

        //subkey Ki = k4i, k4i+1, k4i+2, and k4i+3
        List<Block> preSubkeys = new LinkedList<Block>();

        for (int i = 0; i < 32; i++) {
            long upper = (((long) k[4*i]) << 32 ) | ((long) k[4*i+1]);
            long lower = (((long) k[4*i+2]) << 32 ) | ((long) k[4*i+3]);
            preSubkeys.add(new Block(lower, upper));
        }

        //Permute subkeys
        subkeys = new LinkedList<Block>();
        for (Block key : preSubkeys) {
            subkeys.add(Permutation.initialPermutation(key));
        }
    }

    public Block getSubkey(int subkey) {
        if (subkeys == null || subkey >= subkeys.size()) {
            return null;
        }
        return subkeys.get(subkey);
    }


}
