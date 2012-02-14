public class Serpent{

    private static Block initialPermutation(Block block){
        return permute(block, Constants.initPerm);
    }

    private static Block finalPermutation(Block block){
        return permute(block, Constants.finalPerm);
    }

    private static Block permute(Block block, int[] permutation) {
        Block permuted = new Block(0x0L, 0x0L);

        for (int i = 0; i < 64; i++) {
            long t = (block.getLower() << i) & Constants.mask;
            if (t != Constants.mask) {
                int location = permutation[i];
                if (location > 63) {
                    location -= 64;
                    permuted.setUpper(permuted.getUpper() | (Constants.mask >> location));
                } else {
                    permuted.setLower(permuted.getLower() | (Constants.mask >> location));
                }
            }
                    
        }
        for (int i = 0; i < 64; i++) {
            long t = (block.getUpper() << i) & Constants.mask;
            if (t != Constants.mask) {
                int location = permutation[i+64];
                if (location > 63) {
                    location -= 64;
                    permuted.setUpper(permuted.getUpper() | (Constants.mask >> location));
                } else {
                    permuted.setLower(permuted.getLower() | (Constants.mask >> location));
                }
            }
                    
        }

        return permuted;
    }

    public static Block encrypt(Block text, Block key){
        //Generate round subkeys
        Key serpentKey = new Key(key);

        Block roundInput = initialPermutation(text);

        for(int i = 0; i < Constants.numRounds; i++){
            //XOR round subkey
            long lower = roundInput.getLower() ^ serpentKey.getSubkey(i).getLower();
            long upper = roundInput.getUpper() ^ serpentKey.getSubkey(i).getUpper();
/*
            //Apply S boxes
            for(int j = 0; j < 64; j+= 4){
                mixedUpper |= sBoxes[i % 8][(int)( mixedUpper >> j)] << j;
                mixedLower |= sBoxes[i % 8][(int)( mixedLower >> j)] << j;
            }

            if (numRounds < 31) {
                //Perform linear transformation
                int x0 = (int) mixedUpper >> 32;
                int x1 = (int) mixedUpper >> 0;
                int x2 = (int) mixedLower >> 32;
                int x3 = (int) mixedLower >> 0;

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

                mixedUpper |= x0 << 32;
                mixedUpper |= x1 << 0;
                mixedLower |= x2 << 32;
                mixedLower |= x3 << 0;
            } else {
                //XOR to last subkey
            }
*/

        }
        
        //Finish with final permutation
        return finalPermutation(roundInput);
    }
}
