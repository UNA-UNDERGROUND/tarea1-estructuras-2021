#pragma once
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <type_traits>
#include <vcruntime.h>

template <typename T> class Vector {
	T *arreglo = nullptr;
	size_t longitud = 0;
	size_t cantidad = 0;

  public:
	Vector() = default;
	explicit Vector(std::initializer_list<T> l)
	    : longitud(l.size()), cantidad(l.size()) {

		arreglo = new T[longitud];
		size_t i = 0;
		for (auto val : l) {
			arreglo[i] = val;
			i++;
		}
	}
	explicit Vector(const Vector<T> &v)
	    : longitud(v.longitud), cantidad(v.cantidad) {
		arreglo = new T[longitud];
		for (size_t i = 0; i < cantidad; i++) {
			arreglo[i] = v.arreglo[i];
		}
	}
	explicit Vector(Vector<T> &&v) noexcept
	    : arreglo(std::move(v.arreglo)), longitud(v.longitud),
	      cantidad(v.cantidad) {
		v.cantidad = 0;
		v.longitud = 0;
		v.arreglo = nullptr;
	}

	void resize(size_t len, bool copiar = true) {

		if (len > longitud) {
			T *nuevo = new T[len];
			if (copiar && arreglo) {
				for (size_t i = 0; i < cantidad; i++) {
					nuevo[i] = std::move(arreglo[i]);
				}
			} else {
				cantidad = 0;
			}
			longitud = len;
			delete[] arreglo;
			arreglo = nuevo;
		} else if (len < longitud) {
			T *nuevo = new T[len];
			if (copiar) {
				size_t limite = std::min(cantidad, len);
				for (size_t i = 0; i < limite; i++) {
					nuevo[i] = std::move(arreglo[i]);
				}
				if constexpr (std::is_pointer<T>::value) {
					// eliminamos el resto
					if (len < cantidad) {
						size_t diferiencia = cantidad - len;
						for (size_t i = diferiencia; i < cantidad; i++) {
							delete arreglo[i];
						}
					}
				}
				cantidad = limite;
			} else {
				cantidad = 0;
			}

			delete[] arreglo;
			arreglo = nuevo;
			longitud = len;
		}
	}
	void reserve(size_t len = 0) {
		if (len == 0) {
			len = longitud ? longitud : 5;
		}
		resize(longitud + len);
	}

	void push_back(T val) {
		if (cantidad >= longitud) {
			reserve();
		}
		arreglo[cantidad++] = val;
	}
	void push_back(T &&val) {
		if (cantidad >= longitud) {
			reserve();
		}
		arreglo[cantidad++] = std::move(val);
		if constexpr (std::is_pointer<T>::value) {
			delete val;
		}
	}

	void insert(T &val, size_t pos) {
		if (pos > cantidad) {
			throw std::out_of_range("posicion fuera de limites");
		}
		shift(pos);
		arreglo[pos] = val;
	}
	void insert(T &&val, size_t pos) {
		if (pos > cantidad) {
			throw std::out_of_range("posicion fuera de limites");
		}
		shift(pos);
		arreglo[pos] = std::move(val);
	}

	template <typename... Argumentos>
	void emplace_back(Argumentos... argumentos) {
		if (cantidad >= longitud) {
			reserve();
		}
		if constexpr (std::is_pointer<T>::value) {
			arreglo[cantidad++] =
			    new T(std::forward<Argumentos...>(argumentos...));
		} else {
			arreglo[cantidad++] = T(std::forward<Argumentos...>(argumentos...));
		}
	}

	void pop_back() {
		if (cantidad) {
			erase(size() - 1);
		}
	}
	void erase(size_t pos) {
		for (size_t i = pos; (i + 1) < cantidad; i++) {
			arreglo[i] = std::move(arreglo[i + 1]);
		}
		if constexpr (std::is_pointer<T>::value) {
			delete arreglo[cantidad - 1];
		}
		cantidad--;
	}

	void clear() {
		if constexpr (std::is_pointer<T>::value) {
			for (size_t i = 0; i < cantidad; i++) {
				delete arreglo[i];
			}
		}
		delete[] arreglo;
		arreglo = new T[longitud ? longitud : 1];
		cantidad = 0;
	}

	T &at(size_t pos) {
		if (pos > cantidad) {
			throw std::out_of_range("posicion fuera de limites");
		}
		if constexpr (std::is_pointer<T>::value) {
			return *arreglo[pos];
		}
		return arreglo[pos];
	}
	const T &at(size_t pos) const {
		if (pos > cantidad) {
			throw std::out_of_range("posicion fuera de limites");
		}
		if constexpr (std::is_pointer<T>::value) {
			return *arreglo[pos];
		}
		return arreglo[pos];
	}

	T &operator[](size_t pos) { return at(pos); }
	const T &operator[](size_t pos) const { return at(pos); }

	void swap(size_t posA, size_t posB) {
		if (posA != posB && posA < cantidad && posB < cantidad) {
			std::swap(arreglo[posA], arreglo[posB]);
		}
	}

	size_t size() const { return cantidad; }
	bool empty() const { return cantidad == 0; }

	class iterator {
		T *val = nullptr;

	  public:
		iterator() = default;
		explicit iterator(T &val) {
			if constexpr (std::is_pointer<T>::value) {
				this->val = val;
			} else {
				this->val = &val;
			}
		}
		T &operator*() { return *val; }
		bool operator!=(iterator &o) { return this->val != o.val; }
		iterator &operator++() {
			++val;
			return *this;
		}
		iterator &operator--() {
			--val;
			return *this;
		}
	};

	class const_iterator {
		T const *val = nullptr;

	  public:
		const_iterator() = default;
		explicit const_iterator(const T &val) {
			if constexpr (std::is_pointer<T>::value) {
				this->val = val;
			} else {
				this->val = &val;
			}
		}

		const T &operator*() { return *val; }
		bool operator!=(T &val) { return *this->val != val; }
		const_iterator &operator++() {
			++val;
			return *this;
		}
	};

	iterator begin() { return iterator(arreglo[0]); }
	iterator end() { return iterator(arreglo[cantidad]); }
	const const_iterator begin() const { return const_iterator(arreglo[0]); }
	const const_iterator end() const {
		return const_iterator(arreglo[cantidad]);
	}

	Vector &operator=(const Vector<T> &v) {
		if constexpr (std::is_pointer<T>::value) {
			for (size_t i = 0; i < cantidad; i++) {
				delete arreglo[i];
			}
		}
		cantidad = v.cantidad;
		longitud = v.longitud;
		delete[] arreglo;

		arreglo = new T[longitud];
		for (size_t i = 0; i < cantidad; i++) {
			arreglo[i] = v.arreglo[i];
		}

		return *this;
	}

	Vector &operator=(Vector<T> &&v) noexcept {
		if constexpr (std::is_pointer<T>::value) {
			for (size_t i = 0; i < cantidad; i++) {
				delete arreglo[i];
			}
		}
		cantidad = v.cantidad;
		longitud = v.longitud;
		delete[] arreglo;

		arreglo = std::move(v.arreglo);
		v.cantidad = 0;
		v.longitud = 0;
		v.arreglo = nullptr;

		return *this;
	}

  private:
	// mueve los elementos para insertar un nuevo elemento
	// tambien redimensiona de ser necesario
	void shift(size_t pos) {
		if (pos < cantidad && cantidad == longitud) {
			reserve();
		}
		for (size_t i = cantidad; (i) > pos; i--) {
			arreglo[i] = std::move(arreglo[i - 1]);
		}
	}

  public:
	~Vector() {
		if constexpr (std::is_pointer<T>::value) {
			for (size_t i = 0; i < cantidad; i++) {
				delete arreglo[i];
			}
		}
		delete[] arreglo;
	}
};