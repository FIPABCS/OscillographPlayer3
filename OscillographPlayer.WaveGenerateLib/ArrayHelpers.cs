namespace OscillographPlayer.WaveGenerateLib
{
    internal static class ArrayHelpers
    {

        internal static T[,] BulidMap<T>(ref T[] flatMap, int width, int height)
        {
            if (flatMap.Length != width * height)
            {
                throw new Exception("Can not build map int this size.");
            }

            T[,] map = new T[height, width];

            for (int y = 0, i = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++, i++)
                {
                    map[y, x] = flatMap[i];
                }
            }

            return map;
        }

        internal static T[] FlatMap<T>(ref T[,] map)
        {
            int width = map.GetLength(1),
                height = map.GetLength(0);

            T[] flatMap = new T[width * height];

            for (int y = 0, i = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++, i++)
                {
                    flatMap[i] = map[y, x];
                }
            }

            return flatMap;
        }
    }
}