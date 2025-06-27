using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;

namespace WpfApp2
{
    public class Serializer
    {
        public string Serialize<T>(T obj)
        {
            return JsonSerializer.Serialize(obj);
        }
    }
}
