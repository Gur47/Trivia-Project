using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace WpfApp2
{
    /// <summary>
    /// Interaction logic for GameResultsWindow.xaml
    /// </summary>
    public partial class GameResultsWindow : Window
    {
        public GameResultsWindow(List<PlayerResult> results)
        {
            InitializeComponent();

            var sorted = results
                .OrderByDescending(r => r.correctAnswers)
                .ThenBy(r => r.totalTime)
                .ToList();

            var finalList = new List<PlayerResult>();
            int rank = 1;
            int skip = 1;

            for (int i = 0; i < sorted.Count; i++)
            {
                if (i > 0 &&
                    sorted[i].correctAnswers == sorted[i - 1].correctAnswers &&
                    Math.Abs(sorted[i].totalTime - sorted[i - 1].totalTime) < 0.01)
                {
                    sorted[i].Rank = sorted[i - 1].Rank;
                    skip++;
                }
                else
                {
                    sorted[i].Rank = $"{rank}.";
                    rank += skip;
                    skip = 1;
                }

                finalList.Add(sorted[i]);
            }

            Console.WriteLine("[DEBUG] Received Results:");
            foreach (var player in results)
            {
                Console.WriteLine($"{player.username}, correct: {player.correctAnswers}, time: {player.totalTime}");
            }

            ResultsListView.ItemsSource = finalList;
        }

        private void BackToMenu_Click(object sender, RoutedEventArgs e)
        {
            new MainMenuWindow().Show();
            this.Close();
        }
    }

}
