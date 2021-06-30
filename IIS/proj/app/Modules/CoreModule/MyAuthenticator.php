<?php

//use Nette;

class MyAuthenticator implements Nette\Security\IAuthenticator
{
    /**
     * @inject
     * @var Nette\Database\Context
     */
    public $database;
	private $passwords;

	public function __construct(Nette\Database\Context $database, Nette\Security\Passwords $passwords)
	{
		$this->passwords = $passwords;
	}

	public function authenticate(array $credentials): Nette\Security\IIdentity
	{
	    [$username, $password] = $credentials;

	    $user_row = $this->database->table('user')
            ->where('guest_mail', $username)
            ->fetch();
            
	    if (!$user_row) {
	        throw new Nette\Security\AuthenticationException('User not found.');
	    } elseif ($password != $user_row->password) {
	        throw new Nette\Security\AuthenticationException('Invalid password.');
	    }
	    return new Nette\Security\Identity($user_row->guest_mail, $user_row->role, null);
	}
        
	public function addUser(stdClass $values)
        {
            try {
                if ($this->database->table('guest')->where('mail_address', $values->email)->fetch()){
                    
                    $this->database->query('UPDATE guest SET', [
                        'first_name' => $values->firstname,
                        'last_name' => $values->lastname,
                        'birth_date' => $values->birthdate,
                        'city' => $values->city,
                        'street' => $values->street
                    ],
                    'WHERE mail_address = ?', $values->email);
                    
                    $this->database->table('user')->insert([
                        'password' => $values->password,
                        'role' => 4,
                        'guest_mail' => $values->email
                    ]);
                }else{
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
                }
                    
                
            } catch (Nette\Database\UniqueConstraintViolationException $e) {
                throw new Exception;
            }
        }

    public function removeUser($usr) : void
    {
        $this->database->table('user')
            ->where('mail_address', $usr)
            ->delete();
    }
}
