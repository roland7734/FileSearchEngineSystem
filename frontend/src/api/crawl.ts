import axiosInstance from "./axiosInstance";
import { CrawlRequest } from "../models/crawlRequest";

export const startCrawl = async (crawlRequest: CrawlRequest) => {
  try {
    const response = await axiosInstance.post("/crawl", crawlRequest);
    return response.data;
  } catch (error: any) {
    console.error("Error during crawl:", error);
    const errorMessage =
      (
        (error.response?.data?.error || "") +
        " " +
        (error.response?.data?.details || "")
      ).trim() || "An error occurred during crawl.";

    throw new Error(errorMessage);
  }
};
