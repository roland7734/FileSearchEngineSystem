// api/startCrawl.ts
import axiosInstance from "./axiosInstance";
import { CrawlRequest } from "../models/crawlRequest";

export const startCrawl = async (crawlRequest: CrawlRequest) => {
  try {
    const response = await axiosInstance.post("/crawl", crawlRequest);
    return response.data;
  } catch (error: any) {
    console.error("Error during crawl:", error);
    throw new Error(
      error.response?.data?.message || "An error occurred during crawl."
    );
  }
};
