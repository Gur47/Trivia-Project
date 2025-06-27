using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;

namespace WpfApp2
{
    public class Deserializer
    {
        public T Deserialize<T>(string json)
        {
            return JsonSerializer.Deserialize<T>(json);
        }

        public QuestionResponse DeserializeQuestionResponse(string json)
        {
            return JsonSerializer.Deserialize<QuestionResponse>(json);
        }

        public SubmitAnswerResponse DeserializeSubmitAnswerResponse(string json)
        {
            return JsonSerializer.Deserialize<SubmitAnswerResponse>(json);
        }

        public GameResultsResponse DeserializeGameResultsResponse(string json)
        {
            return JsonSerializer.Deserialize<GameResultsResponse>(json);
        }


    }
}
