<?php

//use Nette;

class AdminModel
{
    use Nette\SmartObject;

    /**
     * @inject
     * @var Nette\Database\Context
     */
    public $database;

	public function addUser(stdClass $values)
    {
        try {
            $this->database->table('guest')->insert([
                'first_name' => $values->firstname,
                'last_name' => $values->lastname,
                'mail_address' => $values->email,
                'birth_date' => $values->birthdate,
                'city' => $values->city,
                'street' => $values->street
            ]);
                
            $this->database->table('user')->insert([
                'password' => $values->password,
                'role' => 4,
                'guest_mail' => $values->email
            ]);
                
        } catch (Nette\Database\UniqueConstraintViolationException $e) {
            throw new Exception;
        }
    }
        
    public function removeUser($usr) : void
    {
        $this->database->table('user')
            ->where('guest_id', $usr)
            ->delete();
    }
}
