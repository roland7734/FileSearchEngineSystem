import React from "react";
import { Bar } from "react-chartjs-2";
import { Box, Typography } from "@mui/material";
import {
  Chart as ChartJS,
  BarElement,
  CategoryScale,
  LinearScale,
  Tooltip,
  Legend,
} from "chart.js";

ChartJS.register(BarElement, CategoryScale, LinearScale, Tooltip, Legend);

interface Props {
  data: { [month: string]: number };
}

const MonthlyHistogram: React.FC<Props> = ({ data }) => {
  const labels = Object.keys(data).sort(); // "01", "02", ...
  const values = labels.map((month) => data[month]);

  const chartData = {
    labels,
    datasets: [
      {
        label: "Files per Month",
        data: values,
        backgroundColor: "#42a5f5",
        borderRadius: 5,
      },
    ],
  };

  const chartOptions = {
    responsive: true,
    plugins: {
      legend: {
        position: "top" as const,
      },
    },
    scales: {
      y: {
        beginAtZero: true,
        ticks: {
          stepSize: 1,
        },
      },
    },
  };

  return (
    <Box mt={5}>
      <Typography variant="h5" fontWeight="bold" gutterBottom>
        Monthly File Distribution
      </Typography>
      <Bar data={chartData} options={chartOptions} />
    </Box>
  );
};

export default MonthlyHistogram;
