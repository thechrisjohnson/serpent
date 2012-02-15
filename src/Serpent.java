public class Serpent{

    public static final int numRounds = 32;

    public static Block encrypt(Block text, Block key){
        //Generate round subkeys
        Key serpentKey = new Key(key);

        Block roundInput = Permutation.initialPermutation(text);

        for(int i = 0; i < numRounds; i++){
            //XOR round subkey
            long lower = roundInput.getLower() ^ serpentKey.getSubkey(i).getLower();
            long upper = roundInput.getUpper() ^ serpentKey.getSubkey(i).getUpper();

            //Apply S boxes
            lower = SBox.sBox(lower, i%8);
            upper = SBox.sBox(upper, i%8);

            if (i < 31) {
                //Break into 32-bit words
                int x0 = (int) (lower >> 32);
                int x1 = (int) lower;
                int x2 = (int) (upper >> 32);
                int x3 = (int) upper;

                //Perform linear transformation
                x0 = x0 << 13;
                x2 = x2 << 3;
                x1 = x1 ^ x0 ^ x2;
                x3 = x3 ^ x2 ^ (x0 << 3);
                x1 = x1 << 1;
                x3 = x3 << 7;
                x0 = x0 ^ x1 ^ x3;
                x2 = x2 ^ x3 ^ (x1 << 7);
                x0 = x0 << 5;
                x2 = x2 << 22;

                //Recombine
                lower = 0x0;
                upper = 0x0;
                lower = (((long) x0) << 32) | ((long) x1);
                upper = (((long) x2) << 32) | ((long) x3);
            } else {
                // XOR last key
                Block lastKey = Permutation.initialPermutation(serpentKey.getSubkey(i+1));
                lower ^= lastKey.getLower();
                upper ^= lastKey.getUpper();
            }

            roundInput.setLower(lower);
            roundInput.setUpper(upper);
        }
        
        //Finish with final permutation
        return Permutation.finalPermutation(roundInput);
    }

    public static void main(String args[]) {
        System.out.println("");
        
    }
}
