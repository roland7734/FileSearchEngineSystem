import axiosInstance from "./axiosInstance";

export const searchFiles = async (query: string) => {
  try {
    const response = await axiosInstance.get(
      `/search?query=${encodeURIComponent(query)}`
    );
    return response.data;
  } catch (error: any) {
    console.error("Error searching files:", error);

    const errorMessage =
      (
        (error.response?.data?.error || "") +
        " " +
        (error.response?.data?.details || "")
      ).trim() || "An error occurred while searching files.";

    throw new Error(errorMessage);
  }
};
