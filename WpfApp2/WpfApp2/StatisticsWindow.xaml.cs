using System;
using System.Collections.Generic;
using System.Text.Json;
using System.Windows;
using System.Windows.Controls;

namespace WpfApp2
{
    public partial class StatisticsWindow : Window
    {
        public StatisticsWindow()
        {
            InitializeComponent();
        }

        private void PersonalStats_Click(object sender, RoutedEventArgs e)
        {
            App.Communicator.SendRequest((byte)RequestCodes.GET_PERSONAL_STATS_REQUEST, "{}");
            string json = App.Communicator.ReceiveResponse();
            var stats = App.Deserializer.Deserialize<UserPersonalStats>(json);

            StatsListView.Items.Clear();
            StatsListView.Items.Add($"👤 Username: {stats.username}");
            StatsListView.Items.Add($"🎮 Games Played: {stats.gamesPlayed}");
            StatsListView.Items.Add($"✅ Correct Answers: {stats.correctAnswers}");
            StatsListView.Items.Add($"❓ Total Answers: {stats.totalAnswers}");
            StatsListView.Items.Add($"📈 Accuracy: {(stats.totalAnswers > 0 ? (stats.correctAnswers * 100 / stats.totalAnswers) : 0)}%");
            StatsListView.Items.Add($"🏆 Wins: {stats.wins}");
            StatsListView.Items.Add($"⏱️ Avg Time per Answer: {stats.averageTime:F2} sec");
        }

        private void GeneralStats_Click(object sender, RoutedEventArgs e)
        {
            App.Communicator.SendRequest((byte)RequestCodes.GET_GENERAL_STATS_REQUEST, "{}");
            string json = App.Communicator.ReceiveResponse();

            try
            {
                var stats = JsonSerializer.Deserialize<GeneralStatsResponse>(json);

                StatsListView.Items.Clear();

                StatsListView.Items.Add("🏆 Top 5 - Most Correct Answers:");
                if (stats.topCorrectAnswers.Count == 0)
                    StatsListView.Items.Add("• No data yet");
                else
                    foreach (var p in stats.topCorrectAnswers)
                        StatsListView.Items.Add($"• {p.username} - {p.value}");

                StatsListView.Items.Add("");
                StatsListView.Items.Add("👑 Top 5 - Most Wins:");
                if (stats.topWins.Count == 0)
                    StatsListView.Items.Add("• No data yet");
                else
                    foreach (var p in stats.topWins)
                        StatsListView.Items.Add($"• {p.username} - {p.value}");

                StatsListView.Items.Add("");
                StatsListView.Items.Add("🎯 Top 5 - Accuracy:");
                if (stats.topAccuracy.Count == 0)
                    StatsListView.Items.Add("• No data yet");
                else
                    foreach (var p in stats.topAccuracy)
                        StatsListView.Items.Add($"• {p.username} - {p.value:P1}");
            }
            catch (Exception ex)
            {
                MessageBox.Show("Failed to load stats.\n" + ex.Message);
            }
        }



        private void BackToMenu_Click(object sender, RoutedEventArgs e)
        {
            new MainMenuWindow().Show();
            this.Close();
        }
    }
}
