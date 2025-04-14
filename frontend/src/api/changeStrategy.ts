import { LoggerStrategyRequest } from "../models/loggerStrategyRequest";
import { LoggerStrategyResponse } from "../models/loggerStrategyResponse";
import axiosInstance from "./axiosInstance";

export const changeLoggerStrategy = async (
  strategy: LoggerStrategyRequest
): Promise<LoggerStrategyResponse> => {
  try {
    const response = await axiosInstance.post<LoggerStrategyResponse>(
      "/logger/strategy",
      strategy
    );
    return response.data;
  } catch (error: any) {
    console.error("Error during changing logger strategy:", error);
    const errorMessage =
      (
        (error.response?.data?.error || "") +
        " " +
        (error.response?.data?.details || "")
      ).trim() || "An error occurred while changing the logger strategy.";

    throw new Error(errorMessage);
  }
};
