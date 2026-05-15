namespace ASOWebApp.Models
{
    public class CircularBuffer<T>
    {
            private readonly T[] _buffer;
            private int _head;
            private int _tail;
            private int _count;

            public CircularBuffer(int capacity)
            {
                if (capacity <= 0) throw new ArgumentException("Capacity must be greater than 0.");
                _buffer = new T[capacity];
            }

            public void Enqueue(T item)
            {
                _buffer[_head] = item;
                _head = (_head + 1) % _buffer.Length;

                if (_count == _buffer.Length)
                {
                    _tail = (_tail + 1) % _buffer.Length;
                }
                else
                {
                    _count++;
                }
            }

            public IEnumerable<T> GetAll()
            {
                int current = _tail;
                for (int i = 0; i < _count; i++)
                {
                    yield return _buffer[current];
                    current = (current + 1) % _buffer.Length;
                }
            }
    }
}
