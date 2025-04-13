import axiosInstance from "./axiosInstance";

export const openFile = async (path: string) => {
  try {
    const response = await axiosInstance.post("/open-file", { path });
    return response.data;
  } catch (error: any) {
    console.error("Error opening file:", error);
    throw new Error(
      "Could not open file: " + (error.message || "Unknown error")
    );
  }
};
