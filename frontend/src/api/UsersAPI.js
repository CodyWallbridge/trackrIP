import axios from "axios";

class UsersAPI {
  static #BASE_URL = "http://localhost:8080/api/users";

  static getUser() {
    return axios.get(this.#BASE_URL + "/", { withCredentials: true });
  }

  static updateUser(firstName, lastName, currentPassword, newPassword) {
    return axios.put(
      this.#BASE_URL + "/",
      { firstName, lastName, currentPassword, newPassword },
      { withCredentials: true }
    );
  }
}
export default UsersAPI;